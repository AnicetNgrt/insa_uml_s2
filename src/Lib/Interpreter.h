#pragma once

#include <string>

using namespace std;

#include "IService.h"
#include "Result.h"
#include "Session.h"

class Interpreter {
public:
    Interpreter(IService& service);
    Result<string, string> interpret(string command) const;

private:
    Result<string, string> cmd_exit() const;
    Result<string, string> cmd_help() const;
    Result<string, string> cmd_reconnect(Command& cmd) const;
    Result<string, string> cmd_measurements(Command& cmd) const;
    Result<string, string> cmd_quality_area(Command& cmd) const;
    Result<string, string> cmd_flag_owner(Command& cmd) const;

    IService& service;
};