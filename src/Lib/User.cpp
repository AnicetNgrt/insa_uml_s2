#include "User.h"

User::User(string username, string password_hash, UserPermissionLevel permission_level)
    : password_hash(password_hash)
    , username(username)
    , permission_level(permission_level)
{
}

UserPermissionLevel user_permission_level_from_string(string s)
{
    if (s.compare("basic") == 0)
        return UserPermissionLevel::BASIC;
    if (s.compare("government") == 0)
        return UserPermissionLevel::GOVERNMENT;
    throw invalid_argument("no enum member could be parsed");
}