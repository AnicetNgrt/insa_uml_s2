#include "CSV.h"

Result<Stream<ParsedCSV_Row>, CSV_ParseError>
csv_parser_from_file(string csv_file_path) {
  ifstream file(csv_file_path);

  if (!file)
    return Err(CSV_ParseError::SOURCE_NOT_READABLE);

  auto receive = [&]() -> Maybe<ParsedCSV_Row> {
    auto data = unordered_map<string, string>();
    return Some(Ok(data));
  };

  auto close = [&]() { file.close(); };

  return Ok(Stream<ParsedCSV_Row>(receive, close));
}