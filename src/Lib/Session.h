#pragma once

#include <string>

using namespace std;

#include "Maybe.h"
#include "Database.h"
#include "User.h"
#include "Cleaner.h"
#include "Owner.h"
#include "Measurement.h"
#include "Sensor.h"

struct Session {
  string const username;
  string const password;
  Database<User> const & const users_db;
  Database<Cleaner> const & const cleaners_db;
  Database<Owner> const & const owners_db;
  Database<Measurement> const & const measurements_db;
  Database<Sensor> const & const sensors_db;

  Maybe<User const*> authed_user;
  unordered_map<string, OwnerFlag> owners_flags;
};

Session from_args(const char** argv, size_t argc);