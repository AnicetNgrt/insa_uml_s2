#pragma once

#include "../Utils/CSV.h"
#include "Database.h"

#include <iostream>

template <typename Data>
class DatabaseCSV : public Database<Data> {
public:
    DatabaseCSV(string path)
        : csv_file_path(path)
    {
    }

    Stream<Data>* filter_and_stream(function<bool(Data const&)> filter) const
    {
        auto maybe_parser = csv_parser_from_file(csv_file_path);
        if (failure(maybe_parser)) {
            return new StreamClosure<Data>();
        }

        auto csv_stream = UnwrapValue(maybe_parser);

        auto receive = [=]() -> Maybe<Data> {
            Data data = Data();
            do {
                auto maybe_row = csv_stream->receive();

                if (none(maybe_row))
                    return None;
                CSV_Row row = Unwrap(maybe_row);

                bool success = data.assign_from_csv(row);
                if (!success)
                    return None;
            } while (!filter(data));

            return Some(data);
        };

        auto close = [=]() { delete csv_stream; };

        return new StreamClosure<Data>(receive, close);
    }

    Stream<Data>* stream() const
    {
        return filter_and_stream([](const Data& _) { return true; });
    }

private:
    string csv_file_path;
};