#include <sstream>

#include "Timestamp.h"

Timestamp timestamp_from_string(string s)
{
    // ex: 2019-01-04 12:00:00
    Timestamp res;
    stringstream time_ss(s);
    string parsed;
    if (time_ss.eof() || time_ss.bad())
        throw invalid_argument("ERROR: invalid date format");
    getline(time_ss, parsed, '-');
    res.year = stoi(parsed);

    if (time_ss.eof())
        throw invalid_argument("ERROR: invalid date format");
    getline(time_ss, parsed, '-');
    res.month = stoi(parsed);

    if (time_ss.eof())
        throw invalid_argument("ERROR: invalid date format");
    getline(time_ss, parsed, ' ');
    res.day = stoi(parsed);

    if (time_ss.eof())
        throw invalid_argument("ERROR: invalid date format");
    getline(time_ss, parsed, ':');
    res.hour = stoi(parsed);

    if (time_ss.eof())
        throw invalid_argument("ERROR: invalid date format");
    getline(time_ss, parsed, ':');
    res.minute = stoi(parsed);

    if (time_ss.eof())
        throw invalid_argument("ERROR: invalid date format");
    getline(time_ss, parsed);
    res.second = stoi(parsed);
    return res;
}

bool timestamp_equal(Timestamp const& t1, Timestamp const& t2) {
    return timestamp_compare(t1, t2) == 0;
}

int timestamp_compare(Timestamp const& t1, Timestamp const& t2) {
    if (t1.year > t2.year) return 1;
    if (t1.year < t2.year) return -1;

    if (t1.month > t2.month) return 1;
    if (t1.month < t2.month) return -1;

    if (t1.day > t2.day) return 1;
    if (t1.day < t2.day) return -1;

    if (t1.minute > t2.minute) return 1;
    if (t1.minute < t2.minute) return -1;

    if (t1.second > t2.second) return 1;
    if (t1.second < t2.second) return -1;

    return 0;
}