#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

#include "IParswordHasher.h"
#include "Database.h"
#include "Maybe.h"
#include "User.h"
#include "Cleaner.h"

class ISession {
public:
  virtual string get_username() const = 0;
  virtual string get_password_hash() const = 0;
  virtual Maybe<User const *> get_authed_user() const = 0;
  virtual Maybe<Database<User> const *> get_users_database() const = 0;
  virtual Maybe<Database<Cleaner> const *> get_cleaners_database() const = 0;
  virtual Maybe<UserFlag> get_user_flag(User const *user) const = 0;

  virtual void set_authed_user(User const *user) = 0;
  virtual void set_user_flag(User const *user, UserFlag flag) = 0;
};