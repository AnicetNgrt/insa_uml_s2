#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

#include "Result.h"
#include "Stream.h"
#include "Timestamp.h"

enum class CSV_ParseError { SOURCE_NOT_READABLE,
    HEADER_NOT_READABLE };

typedef unordered_map<string, string> CSV_Row;
typedef Result<CSV_Row, CSV_ParseError> ParsedCSV_Row;

Result<Stream<ParsedCSV_Row>*, CSV_ParseError>
csv_parser_from_file(string csv_file_path);

class CSV_Assignable {
public:
    virtual bool assign_from_csv(CSV_Row const& csv_row) = 0;
};

#define CSV_TRY_ASSIGN(field, key, string_conversion) \
    try {                                             \
        auto str_val = csv_row.at(key);               \
        field = string_conversion(str_val);           \
    } catch (const std::out_of_range& e) {            \
        return false;                                 \
    } catch (const std::invalid_argument& e) {        \
        return false;                                 \
    }

#define CSV_TRY_ASSIGN_INT(field, key) CSV_TRY_ASSIGN(field, key, std::stoi)

#define CSV_TRY_ASSIGN_STRING(field, key) CSV_TRY_ASSIGN(field, key, )

#define CSV_TRY_ASSIGN_DOUBLE(field, key) CSV_TRY_ASSIGN(field, key, std::stod)

#define CSV_TRY_ASSIGN_TIMESTAMP(field, key) \
    CSV_TRY_ASSIGN(field, key, timestamp_from_string)
