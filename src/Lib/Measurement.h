#pragma once

#include <string>

using namespace std;

#include "CSV.h"
#include "Timestamp.h"

enum class MeasurementType {
  CO2
};

MeasurementType measurement_type_from_string(string s) {
  if (s.compare("CO2") == 0)
    return MeasurementType::CO2;
  throw std::invalid_argument("no enum member could be parsed");
}

class Measurement : public CSV_Assignable {
public:
  Measurement();

  MeasurementType get_type() const { return type; };
  string get_sensor_id() const { return sensor_id; };
  double get_value() const { return value; };
  Timestamp const &get_timestamp() const { return timestamp; };

  bool assign_from_csv(CSV_Row const &csv_row) override {
    CSV_TRY_ASSIGN_DOUBLE(value, "value")
    CSV_TRY_ASSIGN_STRING(sensor_id, "sensor id")
    CSV_TRY_ASSIGN_TIMESTAMP(timestamp, "timestamp")
    CSV_TRY_ASSIGN(type, "type", measurement_type_from_string)
    return true;
  }

private:
  MeasurementType type;
  string sensor_id;
  double value;
  Timestamp timestamp;
};