#pragma once

#include "../Lib/IService.h"

class MockService : public IService {
public:
    MockService();

    Stream<Measurement>* measurements(Maybe<string> sensor_id_filter,
        Maybe<MeasurementType> type_filter,
        Maybe<Timestamp> timestamp_filter) override;
    Result<double, const char*> air_quality_area(double x, double y, double rad,
        Maybe<Timestamp> start,
        Maybe<Timestamp> end) override;
    Result<double, const char*> air_quality(double x, double y,
        Timestamp& at) override;
    Stream<Sensor>* similar_sensors(string sensor_id, int n,
        Maybe<Timestamp> start,
        Maybe<Timestamp> end) override;
    Result<double, const char*> cleaner_efficiency(string cleaner_id) override;
    Result<double, const char*>
    provider_cleaners_efficiency(string provider) override;
    void flag_owner(string owner_id, OwnerFlag flag) override;
    Maybe<const char*> authenticate(string username, string password) override;
    Maybe<User> authenticated_user() override;
};