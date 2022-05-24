#include "Service.h"

Service::Service(Session& session)
    : session(session)
{
}

Stream<Measurement>* Service::measurements(string sensor_id,
    Maybe<MeasurementType> type_filter,
    Maybe<Timestamp> timestamp_filter)
{
    auto filter = [&](const Measurement& m) -> bool {
        bool type_good = some(type_filter) ? m.get_type() == Unwrap(type_filter) : true;
        bool timestamp_good = some(timestamp_filter)
            ? timestamp_equal(m.get_timestamp(), Unwrap(timestamp_filter))
            : true;
        bool sensor_good = m.get_sensor_id() == sensor_id;
        return type_good && timestamp_good && sensor_good;
    };

    return session.measurements_db->filter_and_stream(filter);
}

Result<double, const char*> Service::air_quality_area(double x, double y,
    double rad,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end) {
}

Stream<Sensor>* Service::similar_sensors(string sensor_id, int n,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end) { }

Result<double, const char*> Service::cleaner_efficiency(string cleaner_id) { }

Result<double, const char*> Service::provider_cleaners_efficiency(
    string provider) { }

void Service::flag_owner(string owner_id, OwnerFlag flag)
{
    auto filter = [&](const Owner& owner) -> bool {
        return owner.get_id() == owner_id;
    };
    auto owner_stream = session.owners_db->filter_and_stream(filter);
    if (some(owner_stream->receive())) {
        session.owners_flags.insert(make_pair(owner_id, flag));
    }
}

string hash_password(string password)
{
    return password; // TODO!!!!
}

Maybe<const char*> Service::authenticate(string username, string password)
{
    const Database<User>* db = session.users_db;
    auto filter = [&](const User& user) -> bool {
        return (user.get_username() == username && user.get_password_hash() == hash_password(password));
    };
    auto user_stream = db->filter_and_stream(filter);

    session.authed_user = user_stream->receive();
    if (some(session.authed_user)) {
        return Some("Could not authenticate user");
    }
    return None;
}

Maybe<User> Service::authenticated_user() { return session.authed_user; }
