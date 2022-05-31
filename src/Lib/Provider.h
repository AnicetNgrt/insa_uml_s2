#pragma once

#include <string>

using namespace std;

#include "../Utils/CSV.h"

class Provider : public CSV_Assignable {
public:
    Provider() {};
    Provider(string id): id(id) {};

    string get_id() const { return id; };

    bool assign_from_csv(CSV_Row const& csv_row) override
    {
        CSV_TRY_ASSIGN_STRING(id, "provider id")
        return true;
    }

private:
    string id;
};

enum class ProviderFlag { RELIABLE,
    UNRELIABLE };

ProviderFlag provider_flag_from_string(string s);