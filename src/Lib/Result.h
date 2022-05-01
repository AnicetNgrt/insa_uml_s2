/*
   @file Result.h
*/

/* Usage example

enum class DivisionError {
        DIVIDE_BY_ZERO
};

Result<int, DivisionError> divide(int a, int b) {
    if (b == 0) return Err(DivisionError::DIVIDE_BY_ZERO);
    return Ok(a/b);
}

...

auto result = divide(1, 4);
if (result.is_error) {
        switch (Unwrap(result.error_value)) {
                case DivisionError::DIVIDE_BY_ZERO: std::cout << "Division by
zero" << std::endl;
        }
        return 1;
}
int value = Unwrap(result.success_value);

*/

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