#include <sstream>

#include "CSV.h"

Result<Stream<ParsedCSV_Row>*, CSV_ParseError>
csv_parser_from_file(string csv_file_path) {
  auto file = new ifstream(csv_file_path);

  if (file->bad()) {
    delete file;
    return Err(CSV_ParseError::SOURCE_NOT_READABLE);
  }

  auto rows = new vector<string>();

  string row;
  if (getline(*file, row)) {
    stringstream row_ss(row);
    string column_title;
    while (getline(row_ss, column_title, ',')) {
      rows->push_back(column_title);
    }
  } else {
    delete file;
    return Err(CSV_ParseError::HEADER_NOT_READABLE);
  }

  auto receive = [&]() -> Maybe<ParsedCSV_Row> {
    auto data = unordered_map<string, string>();

    string row;
    if (getline(*file, row)) {
      stringstream row_ss(row);
      string column_value;
      for (size_t i = 0; getline(row_ss, column_value, ',') && i < rows->size(); ++i) {
        if (column_value.length() > 0) {
          data.insert(make_pair(rows->at(i), column_value));
        }
      }
    } else {
      return None;
    }

    return Some(Ok(data));
  };

  auto close = [&]() {
    file->close();
    delete file;
    delete rows;
  };

  return Ok(new StreamClosure<ParsedCSV_Row>(receive, close));
}