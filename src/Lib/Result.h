#pragma once

#include "Maybe.h"

template <typename SuccessType, typename ErrorType> struct Result {
  Maybe<SuccessType> const success_value;
  Maybe<ErrorType> const error_value;
  bool const is_error;
  bool const is_success;
};

#define Ok(value)                                                              \
  { Some(value), None, false, true }
#define Err(error)                                                             \
  { None, Some(error), true, false }
