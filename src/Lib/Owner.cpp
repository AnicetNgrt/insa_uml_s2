#include "Owner.h"

OwnerFlag owner_flag_from_string(string s) {
    if (s.compare("reliable") == 0) return OwnerFlag::RELIABLE;
    if (s.compare("unreliable") == 0) return OwnerFlag::UNRELIABLE;
    throw invalid_argument("no enum member could be parsed");
}