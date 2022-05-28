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

string timestamp_to_string(Timestamp const& t);
Timestamp timestamp_from_string(string s);
bool timestamp_equal(Timestamp const& t1, Timestamp const& t2);
int timestamp_compare(Timestamp const& t1, Timestamp const& t2);