#pragma once

#include <string>

using namespace std;

#include "Cleaner.h"
#include "Database.h"
#include "Maybe.h"
#include "Measurement.h"
#include "Owner.h"
#include "Sensor.h"
#include "User.h"

struct Session {
  string const username;
  string const password;
  Database<User> const &users_db;
  Database<Cleaner> const &cleaners_db;
  Database<Owner> const &owners_db;
  Database<Measurement> const &measurements_db;
  Database<Sensor> const &sensors_db;

  Maybe<User> authed_user;
  unordered_map<string, OwnerFlag> owners_flags;
};

Session from_args(const char **argv, size_t argc);