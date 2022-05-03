#pragma once

#include <string>

using namespace std;

#include "Result.h"
#include "Stream.h"
#include "Measurement.h"
#include "Timestamp.h"
#include "Sensor.h"
#include "Owner.h"
#include "User.h"

class IService {
public:
  virtual Stream<Measurement>* measurements(string sensor_id, Maybe<MeasurementType> type, Maybe<Timestamp> timestamp) = 0;
  virtual Result<double, const char*> air_quality_area(double x, double y, double rad, Maybe<Timestamp> start, Maybe<Timestamp> end) = 0;
  virtual Result<double, const char*> air_quality(double x, double y, Timestamp &at) = 0;
  virtual Stream<Sensor>* similar_sensors(string sensor_id, int n, Maybe<Timestamp> start, Maybe<Timestamp> end) = 0;
  virtual Result<double, const char*> cleaner_efficiency(string cleaner_id) = 0;
  virtual Result<double, const char*> provider_cleaners_efficiency(string provider) = 0;
  virtual void flag_owner(string owner_id, OwnerFlag flag) = 0;
  virtual Maybe<const char*> authenticate(string username, string password) = 0;
  virtual Maybe<User> authenticated_user() = 0;
};