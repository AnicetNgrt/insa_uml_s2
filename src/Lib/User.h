#pragma once

#include <string>

using namespace std;

#include "CSV.h"

enum class UserFlag { RELIABLE, UNRELIABLE };

enum class UserPermissionLevel { BASIC, GOVERNMENT };

class User : public CSV_Assignable {
public:
  User();

  string get_password_hash() const;
  string get_username() const;
  UserPermissionLevel get_permission_level() const;

  bool assign_from_csv(CSV_Row const &csv_row) override;

private:
  string password_hash;
  string username;
  UserPermissionLevel permission_level;
};