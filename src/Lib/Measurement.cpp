#include <sstream>

#include "Measurement.h"

string Measurement::to_string() const
{
    stringstream formatted("");

    formatted << get_sensor_id() << " ";
    switch (get_type()) {
    case MeasurementType::NO2:
        formatted << "NO2";
        break;
    case MeasurementType::O3:
        formatted << "O3";
        break;
    case MeasurementType::PM10:
        formatted << "PM10";
        break;
    case MeasurementType::SO2:
        formatted << "SO2";
        break;
    }
    Timestamp t = get_timestamp();
    formatted << " " << timestamp_to_string(t);
    formatted << " " << get_value();

    return formatted.str();
}