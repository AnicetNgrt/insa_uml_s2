#pragma once

#include <string>

using namespace std;

#include "../Utils/CSV.h"
#include "../Utils/Timestamp.h"
#include "MeasurementType.h"

class Measurement : public CSV_Assignable {
public:
    Measurement() {};
    Measurement(MeasurementType type, string sensor_id, double value, Timestamp timestamp)
        : type(type)
        , sensor_id(sensor_id)
        , value(value)
        , timestamp(timestamp) {};

    MeasurementType get_type() const { return type; };
    string get_sensor_id() const { return sensor_id; };
    double get_value() const { return value; };
    Timestamp const& get_timestamp() const { return timestamp; };

    string to_string() const;

    bool assign_from_csv(CSV_Row const& csv_row) override
    {
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