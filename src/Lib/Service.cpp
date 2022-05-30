#include "Service.h"

Service::Service(Session& session, PasswordHasher password_hasher, AirQualityComputer air_quality_computer)
    : session(session)
    , password_hasher(password_hasher)
    , air_quality_computer(air_quality_computer)
{
}

Stream<Measurement>* Service::measurements(Maybe<string> sensor_id_filter,
    Maybe<MeasurementType> type_filter,
    Maybe<Timestamp> timestamp_filter)
{
    auto filter = [=](const Measurement& m) -> bool {
        bool type_good = some(type_filter) ? m.get_type() == Unwrap(type_filter) : true;
        bool timestamp_good = some(timestamp_filter)
            ? timestamp_equal(m.get_timestamp(), Unwrap(timestamp_filter))
            : true;
        bool sensor_good = some(sensor_id_filter) ? m.get_sensor_id().compare(Unwrap(sensor_id_filter)) == 0 : true;
        return type_good && timestamp_good && sensor_good;
    };

    return session.measurements_db->filter_and_stream(filter);
}

Result<AirQuality, string> Service::air_quality_area(double x, double y,
    double rad,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end)
{
    unordered_map<string, Sensor> cached_sensors;

    auto filter_measurements = [&](const Measurement& m) -> bool {
        // checking timestamps
        bool start_good = some(start) ? timestamp_compare(m.get_timestamp(), Unwrap(start)) > -1 : true;
        bool end_good = some(end) ? timestamp_compare(m.get_timestamp(), Unwrap(end)) < 1 : true;
        if (!start_good || !end_good)
            return false;

        // trying to find the sensor to check position
        string sensor_id = m.get_sensor_id();
        Sensor sensor;
        if (cached_sensors.find(sensor_id) == cached_sensors.end()) { // sensor was not found yet
            // then try to find it in the db
            auto filter_sensors = [&](const Sensor& s) -> bool {
                return sensor_id == s.get_id();
            };

            auto sensors_stream = session.sensors_db->filter_and_stream(filter_sensors);
            auto maybe_sensor = sensors_stream->receive();
            delete sensors_stream;

            if (some(maybe_sensor)) { // ok so we found the sensor
                sensor = Unwrap(maybe_sensor);
                cached_sensors[sensor_id] = sensor; // cache it for future iterations
            } else { // this measurement had an invalid sensor_id, weird
                return false; // just discard it then
            }
        } else { // sensor was already found for another measurement, so just grab it
            sensor = cached_sensors[sensor_id];
        }

        // ok now let's check if the sensor is in range
        double sx = sensor.get_x();
        double sy = sensor.get_y();
        double distanceSquared = (sx - x) * (sx - x) + (sy - y) * (sy - y); // use distance squared because squared root is expensive
        return distanceSquared <= rad * rad; // if it is in range we keep it
    };

    auto measurements_stream = session.measurements_db->filter_and_stream(filter_measurements);
    AirQuality quality = air_quality_computer(measurements_stream);
    delete measurements_stream;

    return Ok(quality);
}

Result<AirQuality, string> Service::air_quality(double x, double y,
    Timestamp& at)
{
    throw "Unimplemented feature";
    return Ok(AirQuality::GOOD);
}

Stream<Sensor>* Service::similar_sensors(string sensor_id, int n,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end)
{
    throw "Unimplemented feature";
    return new StreamClosure<Sensor>();
}

Result<double, string> Service::cleaner_efficiency(string cleaner_id)
{
    throw "Unimplemented feature";
    return Ok(0.0);
}

Result<double, string> Service::provider_cleaners_efficiency(
    string provider)
{
    throw "Unimplemented feature";
    return Ok(0.0);
}

Maybe<FlagError> Service::flag_owner(string owner_id, OwnerFlag flag)
{
    auto user = authenticated_user();
    if (none(user))
        return Some(FlagError::PERMISSION_DENIED);
    if (some(user) && Unwrap(user).get_permission_level() != UserPermissionLevel::GOVERNMENT)
        return Some(FlagError::PERMISSION_DENIED);

    auto filter = [&](const Owner& owner) -> bool {
        return owner.get_id() == owner_id;
    };

    auto owners_stream = session.owners_db->filter_and_stream(filter);
    auto owner = owners_stream->receive();
    delete owners_stream;

    if (some(owner)) {
        session.owners_flags[owner_id] = flag;
    } else {
        return Some(FlagError::OWNER_NOT_FOUND);
    }
    return None;
}

Maybe<OwnerFlag> Service::get_owner_flag(string owner_id)
{
    auto res = session.owners_flags.find(owner_id);
    if (res == session.owners_flags.end()) {
        auto filter = [&](const Owner& owner) -> bool {
            return owner.get_id() == owner_id;
        };

        auto owners_stream = session.owners_db->filter_and_stream(filter);
        auto owner = owners_stream->receive();
        delete owners_stream;

        if (some(owner)) {
            return Some(OwnerFlag::RELIABLE);
        } else {
            return None;
        }
    } else {
        return Some(res->second);
    }
}

string default_password_hasher(string password)
{
    // Obviously this is for demonstration purposes
    // In production, please link a proper cryptographic hashing & salting implementation
    // that has been thoroughly tested and used in industry projects before.
    return password;
}

Maybe<AuthError> Service::authenticate(string username, string password)
{
    const Database<User>* db = session.users_db;
    bool user_found = false;
    auto filter = [&](const User& user) -> bool {
        if (user.get_username().compare(username) == 0) {
            user_found = true;
            if (user.get_password_hash().compare(password_hasher(password)) == 0) {
                return true;
            }
        }
        return false;
    };

    auto users_stream = db->filter_and_stream(filter);
    session.authed_user = users_stream->receive();
    delete users_stream;

    if (none(session.authed_user)) {
        if (user_found) {
            return Some(AuthError::WRONG_PASSWORD);
        } else {
            return Some(AuthError::USER_NOT_FOUND);
        }
    }
    return None;
}

Maybe<User> Service::authenticated_user() { return session.authed_user; }
