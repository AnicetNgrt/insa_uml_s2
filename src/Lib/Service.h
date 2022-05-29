#pragma once

#include "IService.h"
#include "Session.h"

string default_password_hasher(string);

class Service : public IService {
public:
    Service(Session& session, string(*password_hasher)(string) = &default_password_hasher);

    Stream<Measurement>* measurements(Maybe<string> sensor_id_filter,
        Maybe<MeasurementType> type_filter,
        Maybe<Timestamp> timestamp_filter) override;
    Result<double, string> air_quality_area(double x, double y, double rad,
        Maybe<Timestamp> start,
        Maybe<Timestamp> end) override;
    Result<double, string> air_quality(double x, double y,
        Timestamp& at) override;
    Stream<Sensor>* similar_sensors(string sensor_id, int n,
        Maybe<Timestamp> start,
        Maybe<Timestamp> end) override;
    Result<double, string> cleaner_efficiency(string cleaner_id) override;
    Result<double, string>
    provider_cleaners_efficiency(string provider) override;
    void flag_owner(string owner_id, OwnerFlag flag) override;
    Maybe<AuthError> authenticate(string username, string password) override;
    Maybe<User> authenticated_user() override;

private:
    Session& session;
    string(*password_hasher)(string);
};