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

template<typename ValueType>
struct Maybe {
	std::variant<ValueType, std::nullptr_t> const maybe_value;
	bool const is_absent;
};

#define Some(value) { .maybe_value=value, .is_absent=false }
#define None { .maybe_value=nullptr, .is_absent=true }
#define Unwrap(maybe) std::get<0>(maybe.maybe_value)