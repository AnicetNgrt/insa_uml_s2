#include "MeasurementType.h"

MeasurementType measurement_type_from_string(string s)
{
    if (s.compare("O3") == 0)
        return MeasurementType::O3;
    if (s.compare("NO2") == 0)
        return MeasurementType::NO2;
    if (s.compare("SO2") == 0)
        return MeasurementType::SO2;
    if (s.compare("PM10") == 0)
        return MeasurementType::PM10;

    throw invalid_argument("no enum member could be parsed");
}