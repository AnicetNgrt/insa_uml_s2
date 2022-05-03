#pragma once

#include <string>

using namespace std;

struct Timestamp {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
};

Timestamp timestamp_from_string(string s);
bool timestamp_equal(Timestamp &t1, Timestamp &t2);