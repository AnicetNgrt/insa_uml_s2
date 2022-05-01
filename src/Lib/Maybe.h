/*
   @file Maybe.h
*/

/* Usage example

Maybe<int> divide(int a, int b) {
        if (b == 0) return None;
        return Some(a/b);
}

...

auto maybe = divide(1, 4);
if (maybe.is_absent) return 1;
int value = Unwrap(maybe);

*/

#pragma once

#include <variant>

using namespace std;

template <typename ValueType> struct Maybe {
  variant<ValueType, nullptr_t> const maybe_value;
  bool const is_absent;
};

#define Some(value)                                                            \
  { value, false }
#define None                                                                   \
  { nullptr, true }
#define Unwrap(maybe) get<0>(maybe.maybe_value)