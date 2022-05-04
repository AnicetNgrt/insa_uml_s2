#pragma once

#include "Maybe.h"

template <typename SuccessType, typename ErrorType> struct Result {
  Maybe<SuccessType> success_value;
  Maybe<ErrorType> error_value;
};

#define success(result) some(result.success_value)
#define failure(result) some(result.error_value)
#define Ok(value)                                                              \
  { Some(value), None }
#define Err(error)                                                             \
  { None, Some(error) }
