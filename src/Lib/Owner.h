#pragma once

#include <string>

using namespace std;

#include "CSV.h"

class Owner : public CSV_Assignable {
public:
  Owner();

  string get_id() const { return id; };

  bool assign_from_csv(CSV_Row const &csv_row) override;

private:
  string id;
};