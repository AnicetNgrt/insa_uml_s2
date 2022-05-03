#pragma once

template <typename ValueType> struct Maybe {
  ValueType maybe_value[1];
  bool is_absent;
};

#define Some(value)                                                            \
  { {value}, false }
#define None                                                                   \
  { {}, true }
#define Unwrap(maybe) maybe.maybe_value[0]