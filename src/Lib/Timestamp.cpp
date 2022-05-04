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