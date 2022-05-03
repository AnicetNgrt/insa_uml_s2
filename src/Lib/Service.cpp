#include "Service.h"

Service::Service(Session &session) : session(session)
{
}

Stream<Measurement>* Service::measurements()
{
}

Result<double, const char*> Service::air_quality_area(double x, double y, double rad, Maybe<Timestamp> start, Maybe<Timestamp> end)
{
}

Stream<Sensor>* Service::similar_sensors(string sensor_id, int n, Maybe<Timestamp> start, Maybe<Timestamp> end)
{
}

Result<double, const char*> Service::cleaner_efficiency(string cleaner_id)
{
}

Result<double, const char*> Service::provider_cleaners_efficiency(string provider)
{
}

void Service::flag_owner(string owner_id, OwnerFlag flag)
{
    Database<Owner> ow = new DatabaseCSV("dataset/users.csv");
}

Maybe<const char*> Service::authenticate(string username, string password)
{
}

Maybe<User> Service::authenticated_user()
{
    return session.authed_user;
}

