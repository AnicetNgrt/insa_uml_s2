#pragma once

#include "Database.h"
#include "CSV.h"

template<typename Data>
class DatabaseCSV : public Database {
public:
    DatabaseCSV(string path) : csv_file_path(path) {}

    Stream<Data> filter_and_stream(function<bool(Data const&)> filter);
    Stream<Data> stream(); 

private:
    string csv_file_path;
}; 