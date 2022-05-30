#pragma once

#include "Maybe.h"

template <typename SuccessType, typename ErrorType>
struct Result {
    Maybe<SuccessType> success_value;
    Maybe<ErrorType> error_value;
};

#define success(result) some(result.success_value)
#define failure(result) some(result.error_value)
#define UnwrapValue(result) Unwrap(result.success_value)
#define UnwrapError(result) Unwrap(result.error_value)
#define Ok(value)         \
    {                     \
        Some(value), None \
    }
#define Err(error)        \
    {                     \
        None, Some(error) \
    }
#define to_maybe(result) result.success_value
#define map_success(result, mapper) Ok(mapper(UnwrapValue(result)))
#define map_error(result, mapper) Err(mapper(UnwrapError(result)))
#define ok_from(result) Ok(UnwrapValue(result))
#define err_from(result) Err(UnwrapError(result))
#define is_error(result, expected) (failure(result) ? UnwrapError(result) == expected : false)