#pragma once

#include "Maybe.h"

template <typename SuccessType, typename ErrorType> struct Result {
  Maybe<SuccessType> success_value;
  Maybe<ErrorType> error_value;
  bool is_error;
  bool is_success;
};

#define Ok(value)                                                              \
  { Some(value), None, false, true }
#define Err(error)                                                             \
  { None, Some(error), true, false }
