#include <sstream>

#include "Measurement.h"

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

    throw std::invalid_argument("no enum member could be parsed");
}

string Measurement::to_string() const {
    stringstream formatted("");
    
    formatted << get_sensor_id() << " ";
    switch (get_type()) {
        case MeasurementType::NO2: formatted << "NO2"; break;
        case MeasurementType::O3: formatted << "O3"; break;
        case MeasurementType::PM10: formatted << "PM10"; break;
        case MeasurementType::SO2: formatted << "SO2"; break;
    }
    Timestamp t = get_timestamp();
    formatted << " " << t.day << "/" << t.month << "/" << t.year << ":" << t.hour << ":" << t.minute << ":" << t.second;
    formatted << " " << get_value() << endl;

    return formatted.str();
}