#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

#include "Result.h"
#include "Stream.h"

enum class CSV_ParseError { 
  SOURCE_NOT_READABLE,
  HEADER_NOT_READABLE
};

typedef unordered_map<string, string> CSV_Row;
typedef Result<CSV_Row, CSV_ParseError> ParsedCSV_Row;

Result<Stream<ParsedCSV_Row>*, CSV_ParseError>
csv_parser_from_file(string csv_file_path);

class CSV_Assignable {
public:
  virtual bool assign_from_csv(CSV_Row const &csv_row) = 0;
};