#pragma once

#include "CSV.h"
#include "Database.h"

template <typename Data> 
class DatabaseCSV : public Database<Data> {
public:
  DatabaseCSV(string path) : csv_file_path(path) {}

  Stream<Data> *filter_and_stream(function<bool(Data const &)> filter) {
    auto maybe_parser = csv_parser_from_file(csv_file_path);
    if (maybe_parser.is_error) {
      return new StreamClosure<Data>();
    }

    auto csv_stream = Unwrap(maybe_parser.success_value);

    auto receive = [&]() -> Maybe<Data> {
      Data data();
      do {
        auto maybe_parsed_row = csv_stream->receive();
        if (maybe_parsed_row.is_absent)
          return None;
        ParsedCSV_Row parsed_row = Unwrap(maybe_parsed_row);
        if (parsed_row.is_error)
          return None;
        CSV_Row row = Unwrap(parsed_row.success_value);

        bool success = data.assign_from_csv(row);
        if (!success)
          return None;
      } while (!filter(data));

      return Some(data);
    };

    return new StreamClosure<Data>(receive, [&]() { delete csv_stream; });
  }

  Stream<Data> *stream() {
    return filter_and_stream([]() { return true; });
  }

private:
  string csv_file_path;
};