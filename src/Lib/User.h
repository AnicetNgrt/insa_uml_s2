#pragma once

#include <string>

using namespace std;

#include "CSV.h"

enum class UserPermissionLevel { BASIC, GOVERNMENT };

UserPermissionLevel user_permission_level_from_string(string s);

class User : public CSV_Assignable {
public:
  User();

  string get_password_hash() const;
  string get_username() const;
  UserPermissionLevel get_permission_level() const;

  bool assign_from_csv(CSV_Row const &csv_row) override {
    CSV_TRY_ASSIGN_STRING(password_hash, "password hash")
    CSV_TRY_ASSIGN_STRING(username, "username")
    CSV_TRY_ASSIGN(permission_level, "permission level",
                   user_permission_level_from_string)
    return true;
  }

private:
  string password_hash;
  string username;
  UserPermissionLevel permission_level;
};