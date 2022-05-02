#pragma once

#include "CSV.h"
#include "Database.h"

template <typename Data> class DatabaseCSV : public Database {
public:
  DatabaseCSV(string path) : csv_file_path(path) {}

  Stream<Data> *filter_and_stream(function<bool(Data const &)> filter) {
    auto maybe_parser = csv_parser_from_file(csv_file_path);
    if (maybe_parser.is_error) {
      return Stream();
    }

    auto csv_stream = Unwrap(maybe_parser.success_value);

    auto receive = [&]() -> Maybe<Data> {
      Data data();
      do {
        auto maybe_parsed_row = csv_stream.receive();
        if (maybe_parsed_row.is_absent)
          return None;
        ParsedCSV_Row parsed_row = Unwrap(maybe_row);
        if (parsed_row.is_error)
          return None;
        CSV_Row row = Unwrap(parsed_row.success_value);

        bool success = data.assign_from_csv(row);
        if (!success)
          return None;
      } while (!filter(data));

      return Some(data);
    };

    auto close = [&]() { delete csv_stream; };

    return new Stream(receive, close);
  }

  Stream<Data> *stream();

private:
  string csv_file_path;
};