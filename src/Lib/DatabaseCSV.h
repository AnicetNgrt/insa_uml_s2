#pragma once

#include "CSV.h"
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

        auto csv_stream = Unwrap(maybe_parser.success_value);

        auto receive = [=]() -> Maybe<Data> {
            Data data = Data();
            do {
                auto maybe_parsed_row = csv_stream->receive();

                if (none(maybe_parsed_row))
                    return None;
                ParsedCSV_Row parsed_row = Unwrap(maybe_parsed_row);
                if (failure(parsed_row))
                    return None;
                CSV_Row row = Unwrap(parsed_row.success_value);

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