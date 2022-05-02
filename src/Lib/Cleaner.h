#pragma once

#include <string>

using namespace std;

#include "CSV.h"
#include "Timestamp.h"

class Cleaner : public CSV_Assignable {
public:
  Cleaner();

  string get_id() const;
  string get_provider() const;
  double get_x() const;
  double get_y() const;
  Timestamp const &get_timestamp() const;

  bool assign_from_csv(CSV_Row const &csv_row) override;

private:
  string id;
  string provider;
  double x;
  double y;
  Timestamp timestamp;
};