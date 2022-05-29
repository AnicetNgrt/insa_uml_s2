#include "Service.h"

Service::Service(Session& session, string(*password_hasher)(string))
    : session(session), password_hasher(password_hasher)
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

Result<double, string> Service::air_quality_area(double x, double y,
    double rad,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end) {
}

Result<double, string> Service::air_quality(double x, double y,
    Timestamp& at) {

}

Stream<Sensor>* Service::similar_sensors(string sensor_id, int n,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end) { }

Result<double, string> Service::cleaner_efficiency(string cleaner_id) { }

Result<double, string> Service::provider_cleaners_efficiency(
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

string default_password_hasher(string password)
{
    // Obviously this is for demonstration purposes
    // In production, please link a proper cryptographic hashing & salting implementation
    // that has been thoroughly tested and used by industry projects before.
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
    auto user_stream = db->filter_and_stream(filter);

    session.authed_user = user_stream->receive();
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
