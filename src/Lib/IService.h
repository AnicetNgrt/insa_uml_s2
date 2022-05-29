#pragma once

#include <string>

using namespace std;

#include "Measurement.h"
#include "Owner.h"
#include "Result.h"
#include "Sensor.h"
#include "Stream.h"
#include "Timestamp.h"
#include "User.h"

enum class AuthError {
    USER_NOT_FOUND,
    WRONG_PASSWORD
};

class IService {
public:
    virtual Stream<Measurement>* measurements(Maybe<string> sensor_id_filter, Maybe<MeasurementType> type_filter, Maybe<Timestamp> timestamp_filter) = 0;
    virtual Result<double, string> air_quality_area(double x, double y, double rad, Maybe<Timestamp> start, Maybe<Timestamp> end) = 0;
    virtual Result<double, string> air_quality(double x, double y, Timestamp& at) = 0;
    virtual Stream<Sensor>* similar_sensors(string sensor_id, int n, Maybe<Timestamp> start, Maybe<Timestamp> end) = 0;
    virtual Result<double, string> cleaner_efficiency(string cleaner_id) = 0;
    virtual Result<double, string> provider_cleaners_efficiency(string provider) = 0;
    virtual void flag_owner(string owner_id, OwnerFlag flag) = 0;
    virtual Maybe<AuthError> authenticate(string username, string password) = 0;
    virtual Maybe<User> authenticated_user() = 0;
};