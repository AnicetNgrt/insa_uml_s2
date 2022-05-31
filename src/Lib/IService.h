#pragma once

#include <string>

using namespace std;

#include "Measurement.h"
#include "Provider.h"
#include "../Utils/Result.h"
#include "Sensor.h"
#include "../Utils/Stream.h"
#include "../Utils/Timestamp.h"
#include "User.h"
#include "AirQuality.h"

enum class AuthError {
    USER_NOT_FOUND,
    WRONG_PASSWORD
};

enum class FlagError {
    PROVIDER_NOT_FOUND,
    PERMISSION_DENIED
};

class IService {
public:
    virtual Stream<Measurement>* measurements(Maybe<string> sensor_id_filter, Maybe<MeasurementType> type_filter, Maybe<Timestamp> timestamp_filter) = 0;
    virtual Result<AirQuality, string> air_quality_area(double x, double y, double rad, Maybe<Timestamp> start, Maybe<Timestamp> end) = 0;
    virtual Result<AirQuality, string> air_quality(double x, double y, Timestamp& at) = 0;
    virtual Stream<Sensor>* similar_sensors(string sensor_id, int n, Maybe<Timestamp> start, Maybe<Timestamp> end) = 0;
    virtual Result<double, string> cleaner_efficiency(string cleaner_id) = 0;
    virtual Result<double, string> provider_cleaners_efficiency(string provider) = 0;
    virtual Maybe<FlagError> flag_provider(string provider_id, ProviderFlag flag) = 0;
    virtual Maybe<ProviderFlag> get_provider_flag(string provider_id) = 0;
    virtual Maybe<AuthError> authenticate(string username, string password) = 0;
    virtual Maybe<User> authenticated_user() = 0;
};