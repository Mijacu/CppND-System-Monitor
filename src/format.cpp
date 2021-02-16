#include <string>

#include "format.h"

#define HOUR 3600
#define MIN 60

using std::string;

string Format::ElapsedTime(long s) { 
    string elapsed_time, hours_string, minutes_string, seconds_string;

    int h = s / HOUR;
    s -= h *  HOUR;

    int m = s / MIN;
    s -= m * MIN;

    hours_string = h < 10 ?  "0" + std::to_string(h) : std::to_string(h);
    minutes_string = m < 10 ?  ":0" + std::to_string(m) : ":" + std::to_string(m);
    seconds_string = s < 10 ?  ":0" + std::to_string(s) : ":" + std::to_string(s);
    elapsed_time = hours_string + minutes_string + seconds_string;
    return elapsed_time;
}