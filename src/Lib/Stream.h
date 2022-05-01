#pragma once

#include <functional>
#include "Maybe.h"

template<typename ItemType>
class Stream {
public:
    Stream(std::function<Maybe<ItemType>()> const next_fn);
    Maybe<ItemType> next() const {
        return next_fn();
    }

private:
    std::function<Maybe<ItemType>()> const next_fn;
};