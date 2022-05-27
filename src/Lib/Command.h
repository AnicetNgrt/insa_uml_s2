#pragma once

#include <functional>
#include <string>

#include "./Measurement.h"
#include "./Result.h"
#include "./Timestamp.h"

using namespace std;

enum class ArgError {
    ARG_NOT_FOUND,
    VALUE_NOT_FOUND,
    VALUE_NOT_PARSABLE
};

typedef vector<string> Args;

class Command {
public:
    Command(string command);

    Result<string, ArgError> find_arg(string arg_name) const;
    Result<Timestamp, ArgError> find_timestamp(string arg_name) const;
    Result<MeasurementType, ArgError> find_measurement_type(string arg_name) const;
    Result<double, ArgError> find_double(string arg_name) const;
    
    string get_name() const;

private:
    template <typename T>
    Result<T, ArgError> Command::find_arg(string arg_name, T (*conversion)(string)) const
    {
        Result<string, ArgError> arg = find_arg(arg_name);
        try {
            return map_success(arg, conversion);
        } catch (exception e) {
            return Err(ArgError::VALUE_NOT_PARSABLE);
        }
    }

    Args args;
    string name;
};

function<string(ArgError)> arg_error_to_string(string arg, string friendly_name);