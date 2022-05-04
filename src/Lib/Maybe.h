#pragma once

template <typename ValueType>
struct Maybe {
    ValueType maybe_value[1];
    bool is_some;
};

#define some(maybe) maybe.is_some
#define none(maybe) !maybe.is_some
#define Some(value)     \
    {                   \
        { value }, true \
    }
#define None      \
    {             \
        {}, false \
    }
#define Unwrap(maybe) maybe.maybe_value[0]