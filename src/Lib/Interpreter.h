#pragma once

#include <string>

using namespace std;

#include "Result.h"
#include "IService.h"
#include "Session.h"

class Interpreter {
public:
  Interpreter(IService &service);
  Result<string, string> interpret(string command) const;

private:
  IService &service;
};