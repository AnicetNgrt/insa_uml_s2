#pragma once

#include <string>

using namespace std;

#include "../Utils/CSV.h"

class Owner : public CSV_Assignable {
public:
    Owner() {};
    Owner(string id): id(id) {};

    string get_id() const { return id; };

    bool assign_from_csv(CSV_Row const& csv_row) override
    {
        CSV_TRY_ASSIGN_STRING(id, "id")
        return true;
    }

private:
    string id;
};

enum class OwnerFlag { RELIABLE,
    UNRELIABLE };

OwnerFlag owner_flag_from_string(string s);