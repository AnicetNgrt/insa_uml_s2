#pragma once

template <typename ValueType>
struct Maybe {
    ValueType maybe_value[1];
    bool is_some;
};

#define some(maybe) (maybe).is_some
#define none(maybe) !(maybe).is_some
#define Some(value)     \
    {                   \
        { value }, true \
    }
#define None      \
    {             \
        {}, false \
    }
#define Unwrap(maybe) (maybe).maybe_value[0]

// If maybe is Some(value) replace by mapper(value), otherwise replace by default
// Useful for mapping from maybe value to result value
#define map_or_default(maybe, mapper, default) (some((maybe)) ? mapper(Unwrap((maybe))) : (default))