#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

#include "Result.h"
#include "Stream.h"

enum class CSV_ParseError { SOURCE_NOT_READABLE };

typedef unordered_map<string, string> CSV_Row;
typedef Result<CSV_Row, CSV_ParseError> ParsedCSV_Row;

Result<Stream<ParsedCSV_Row>, CSV_ParseError>
csv_parser_from_file(string csv_file_path);