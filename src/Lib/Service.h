#pragma once

#include "IService.h"
#include "Session.h"

typedef string (*PasswordHasher)(string);
string default_password_hasher(string);

class Service : public IService {
public:
    Service(Session& session, PasswordHasher password_hasher = &default_password_hasher, AirQualityComputer air_quality_computer = &air_quality_compute);

    Stream<Measurement>* measurements(Maybe<string> sensor_id_filter,
        Maybe<MeasurementType> type_filter,
        Maybe<Timestamp> timestamp_filter) override;
    Result<AirQuality, string> air_quality_area(double x, double y, double rad,
        Maybe<Timestamp> start,
        Maybe<Timestamp> end) override;
    Result<AirQuality, string> air_quality(double x, double y,
        Timestamp& at) override;
    Stream<Sensor>* similar_sensors(string sensor_id, int n,
        Maybe<Timestamp> start,
        Maybe<Timestamp> end) override;
    Result<double, string> cleaner_efficiency(string cleaner_id) override;
    Result<double, string>
    provider_cleaners_efficiency(string provider) override;
    Maybe<FlagError> flag_provider(string provider_id, ProviderFlag flag) override;
    Maybe<ProviderFlag> get_provider_flag(string provider_id);
    Maybe<AuthError> authenticate(string username, string password) override;
    Maybe<User> authenticated_user() override;

private:
    Session& session;
    PasswordHasher password_hasher;
    AirQualityComputer air_quality_computer;
};