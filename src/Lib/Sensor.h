#pragma once

#include <string>

using namespace std;

#include "CSV.h"

class Sensor : public CSV_Assignable {
public:
  Sensor();

  string get_id() const { return id; };
  double get_x() const { return x; };
  double get_y() const { return y; };

  bool assign_from_csv(CSV_Row const &csv_row) override;

private:
  string id;
  double x;
  double y;
};