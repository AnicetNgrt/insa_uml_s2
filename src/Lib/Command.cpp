#include <sstream>

#include "Command.h"

static double double_from_string(string s);

Command::Command(string command)
    : name("")
    , args()
{
    char buffer[1024];
    size_t len = 0;
    char quote = 0;

    for (size_t i = 0; i < command.size(); i++) {
        char c = command.at(i);
        if (c == '\'' || c == '"') {
            if (quote == 0) {
                quote = c;
            } else {
                if (name.size() == 0) {
                    name = string(buffer, len);
                } else {
                    args.push_back(string(buffer, len));
                }
                len = 0;
                quote = 0;
            }
        } else if (c == ' ' && quote == 0 && len > 0) {
            if (name.size() == 0) {
                name = string(buffer, len);
            } else {
                args.push_back(string(buffer, len));
            }
            len = 0;
        } else if (c == ' ' && quote == 0 && len <= 0) {
            continue;
        } else {
            buffer[len++] = c;
        }
    }

    if (len > 0) {
        if (name.size() == 0) {
            name = string(buffer, len);
        } else {
            args.push_back(string(buffer, len));
        }
    }
}

Command::Command(const char** argv, size_t argc)
    : name("")
    , args()
{
    if (argc >= 1)
        name = argv[0];
    for (size_t i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
}

Result<string, ArgError> Command::find_arg(string arg_name) const
{
    bool prev_is_arg;
    for (size_t i = 0; i < args.size(); i++) {
        if (prev_is_arg) {
            if (args[i][0] == '-' && !atoi(args[i].c_str())) {
                return Err(ArgError::VALUE_NOT_FOUND);
            } else {
                return Ok(args[i]);
            }
        }
        if (args[i] == arg_name) {
            prev_is_arg = true;
        }
    }
    if (prev_is_arg) {
        return Err(ArgError::VALUE_NOT_FOUND);
    } else {
        return Err(ArgError::ARG_NOT_FOUND);
    }
}

string Command::get_name() const
{
    return name;
}

Args const& Command::get_args() const
{
    return args;
}

Result<Timestamp, ArgError> Command::find_timestamp(string arg_name) const
{
    return find_arg(arg_name, &timestamp_from_string);
}

Result<MeasurementType, ArgError> Command::find_measurement_type(string arg_name) const
{
    return find_arg(arg_name, &measurement_type_from_string);
}

Result<OwnerFlag, ArgError> Command::find_owner_flag(string arg_name) const
{
    return find_arg(arg_name, &owner_flag_from_string);
}

Result<double, ArgError> Command::find_double(string arg_name) const
{
    return find_arg(arg_name, &double_from_string);
}

function<string(ArgError)> arg_error_to_string(string arg_name, string friendly_name)
{
    return [=](ArgError e) -> string {
        switch (e) {
        case ArgError::ARG_NOT_FOUND:
            return friendly_name + " (argument " + arg_name + ") was not found";
        case ArgError::VALUE_NOT_FOUND:
            return friendly_name + " (argument " + arg_name + ") had no value given";
        case ArgError::VALUE_NOT_PARSABLE:
            return friendly_name + " (argument " + arg_name + ") had an invalid value";
        }
    };
}

static double double_from_string(string s)
{
    return stod(s);
}