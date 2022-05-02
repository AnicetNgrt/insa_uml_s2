#pragma once

#include <string>

using namespace std;

#include "CSV.h"
#include "Timestamp.h"

enum class MeasurementType {
  CO2
};

class Measurement : public CSV_Assignable {
public:
  Measurement();

  MeasurementType get_type() const { return type; };
  string get_sensor_id() const { return sensor_id; };
  double get_value() const { return value; };
  Timestamp const &get_timestamp() const { return timestamp; };

  bool assign_from_csv(CSV_Row const &csv_row) override;

private:
  MeasurementType type;
  string sensor_id;
  double value;
  Timestamp timestamp;
};