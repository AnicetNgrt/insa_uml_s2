#pragma once

#include <string>

using namespace std;

class IPasswordHasher {
public:
  virtual string hash(string password) = 0;
  virtual bool verify(string password, string hash) = 0;
};