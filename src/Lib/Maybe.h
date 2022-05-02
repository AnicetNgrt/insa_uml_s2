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

#include <optional>

using namespace std;

template <typename ValueType> struct Maybe {
  ValueType const maybe_value[1];
  bool const is_absent;
};

#define Some(value)                                                            \
  { {value}, false }
#define None                                                                   \
  { {}, true }
#define Unwrap(maybe) maybe.maybe_value[0]