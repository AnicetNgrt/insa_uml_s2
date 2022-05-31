#pragma once

#include <string>

using namespace std;

#include "IService.h"
#include "../Utils/Result.h"
#include "Session.h"
#include "Command.h"

class Interpreter {
public:
    Interpreter(IService& service);
    Result<string, string> interpret(string command) const;

private:
    Result<string, string> cmd_exit() const;
    Result<string, string> cmd_help() const;
    Result<string, string> cmd_login(Command& cmd) const;
    Result<string, string> cmd_measurements(Command& cmd) const;
    Result<string, string> cmd_quality_area(Command& cmd) const;
    Result<string, string> cmd_flag_provider(Command& cmd) const;
    Result<string, string> cmd_provider_flag(Command& cmd) const;

    IService& service;
};