#include "MockService.h"

MockService::MockService() {
}

Stream<Measurement>* MockService::measurements(string sensor_id,
    Maybe<MeasurementType> type_filter,
    Maybe<Timestamp> timestamp_filter)
{
    vector<Measurement> measurements = {
        Measurement(MeasurementType::NO2, "A", 1, timestamp_from_string("2016-02-03 12:00:00")),
        Measurement(MeasurementType::PM10, "A", 1, timestamp_from_string("2016-02-03 13:00:00")),
        Measurement(MeasurementType::NO2, "A", 1, timestamp_from_string("2016-02-03 14:00:00")),
        Measurement(MeasurementType::O3, "Z", 1, timestamp_from_string("2016-02-03 14:00:00"))
    };
    return new StreamVector<Measurement>(measurements);
}

Result<double, const char*> MockService::air_quality_area(double x, double y,
    double rad,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end)
{
}

Result<double, const char*> MockService::air_quality(double x, double y,
    Timestamp& at)
{
}

Stream<Sensor>* MockService::similar_sensors(string sensor_id, int n,
    Maybe<Timestamp> start,
    Maybe<Timestamp> end) { }

Result<double, const char*> MockService::cleaner_efficiency(string cleaner_id) { }

Result<double, const char*> MockService::provider_cleaners_efficiency(
    string provider) { }

void MockService::flag_owner(string owner_id, OwnerFlag flag)
{
}

Maybe<const char*> MockService::authenticate(string username, string password)
{
}

Maybe<User> MockService::authenticated_user()
{
}
