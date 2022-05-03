#include "User.h"

UserPermissionLevel user_permission_level_from_string(string s) {
  if (s.compare("basic") == 0)
    return UserPermissionLevel::BASIC;
  if (s.compare("government") == 0)
    return UserPermissionLevel::GOVERNMENT;
  throw std::invalid_argument("no enum member could be parsed");
}