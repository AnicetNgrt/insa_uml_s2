#pragma once

#include "../Utils/Database.h"

template <typename Data>
class MockDatabase : public Database<Data> {
public:
    MockDatabase(vector<Data> data)
        : data(data)
    {
    }

    Stream<Data>* filter_and_stream(function<bool(Data const&)> filter) const
    {
        Stream<Data>* st = stream();
        auto receive = [=]() -> Maybe<Data> {
            Maybe<Data> value = None;
            do {
                value = st->receive();
                if (none(value))
                    return None;
            } while (!filter(Unwrap(value)));

            return value;
        };

        return new StreamClosure<Data>(receive);
    }

    Stream<Data>* stream() const
    {
        return new StreamVector<Data>(data);
    }

private:
    vector<Data> data;
};