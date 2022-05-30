#pragma once

#include <functional>
#include <string>

#include "./Measurement.h"
#include "./Owner.h"
#include "../Utils/Result.h"
#include "../Utils/Timestamp.h"

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
    Command(const char** argv, size_t argc);

    Result<string, ArgError> find_arg(string arg_name) const;
    Result<Timestamp, ArgError> find_timestamp(string arg_name) const;
    Result<MeasurementType, ArgError> find_measurement_type(string arg_name) const;
    Result<OwnerFlag, ArgError> find_owner_flag(string arg_name) const;
    Result<double, ArgError> find_double(string arg_name) const;

    string get_name() const;
    Args const& get_args() const;

private:
    template <typename T>
    Result<T, ArgError> find_arg(string arg_name, T (*conversion)(string)) const
    {
        Result<string, ArgError> arg = find_arg(arg_name);
        try {
            if (success(arg)) {
                return map_success(arg, conversion);
            } else {
                return err_from(arg);
            }
        } catch (const std::out_of_range& e) {
            return Err(ArgError::VALUE_NOT_PARSABLE);
        } catch (const std::invalid_argument& e) {
            return Err(ArgError::VALUE_NOT_PARSABLE);
        }
    }

    string name;
    Args args;
};

function<string(ArgError)> arg_error_to_string(string arg, string friendly_name);