#pragma once

#include <string>
#include <stdexcept>

using namespace std;

enum class MeasurementType {
    O3,
    NO2,
    SO2,
    PM10
};

MeasurementType measurement_type_from_string(string s);
