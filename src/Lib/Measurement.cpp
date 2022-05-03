#include "Measurement.h"

MeasurementType measurement_type_from_string(string s) {
  if (s.compare("CO2") == 0)
    return MeasurementType::CO2;
  throw std::invalid_argument("no enum member could be parsed");
}