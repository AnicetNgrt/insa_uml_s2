#include <sstream>

#include "Command.h"

Command::Command(string command)
    : name()
    , args()
{
    istringstream iss(command);
    getline(iss, name, ' ');
    if (iss.good()) {
        string s;
        while (getline(iss, s, ' ')) {
            args.push_back(s);
        }
    }
}

Result<string, ArgError> Command::find_arg(string arg_name) const
{
    bool prev_is_arg;
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == arg_name) {
            prev_is_arg = true;
        }
        if (prev_is_arg) {
            if (args[i][0] == '-' && !atoi(args[i].c_str())) {
                return Err(ArgError::VALUE_NOT_FOUND);
            } else {
                return Ok(args[i]);
            }
        }
    }
    return Err(ArgError::ARG_NOT_FOUND);
}

function<string(ArgError)> arg_error_to_string(string arg_name, string friendly_name)
{
    return function([=](ArgError e) -> string {
        switch (e) {
        case ArgError::ARG_NOT_FOUND:
            return friendly_name + " (argument " + arg_name + ") was not found";
        case ArgError::VALUE_NOT_FOUND:
            return friendly_name + " (argument " + arg_name + ") had no value given";
        case ArgError::VALUE_NOT_PARSABLE:
            return friendly_name + " (argument " + arg_name + ") had an invalid value";
        }
    });
}

static double double_from_string(string s)
{
    return atof(s.c_str());
}

string Command::get_name() const {
    return name;
}

Result<Timestamp, ArgError> Command::find_timestamp(string arg_name) const
{
    return find_arg(arg_name, &timestamp_from_string);
}

Result<MeasurementType, ArgError> Command::find_measurement_type(string arg_name) const
{
    return find_arg(arg_name, &measurement_type_from_string);
}

Result<double, ArgError> Command::find_double(string arg_name) const
{
    return find_arg(arg_name, &double_from_string);
}