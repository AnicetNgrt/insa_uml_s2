#pragma once

#include <string>

using namespace std;

#include "../Utils/CSV.h"
#include "../Utils/Timestamp.h"

class Cleaner : public CSV_Assignable {
public:
    Cleaner() {};

    string get_id() const { return id; };
    double get_x() const { return x; };
    double get_y() const { return y; };
    Timestamp const& get_start() const { return start; };
    Timestamp const& get_stop() const { return stop; };

    bool assign_from_csv(CSV_Row const& csv_row) override
    {
        CSV_TRY_ASSIGN_DOUBLE(x, "x")
        CSV_TRY_ASSIGN_DOUBLE(y, "y")
        CSV_TRY_ASSIGN_STRING(id, "id")
        CSV_TRY_ASSIGN_TIMESTAMP(start, "start")
        CSV_TRY_ASSIGN_TIMESTAMP(stop, "stop")
        return true;
    }

private:
    string id;
    double x;
    double y;
    Timestamp start;
    Timestamp stop;
};