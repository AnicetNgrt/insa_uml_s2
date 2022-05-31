#include "Provider.h"

ProviderFlag provider_flag_from_string(string s) {
    if (s.compare("reliable") == 0) return ProviderFlag::RELIABLE;
    if (s.compare("unreliable") == 0) return ProviderFlag::UNRELIABLE;
    throw invalid_argument("no enum member could be parsed");
}