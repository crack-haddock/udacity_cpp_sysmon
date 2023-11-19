#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    // TODO format as 00:00:00 when uptime working better
    
    if (seconds == 0)
        return string("99:99:99");

    long secs = seconds % 60; //std::cout << "secs moduloed= " << secs << std::endl;
    int mins = (seconds / 60) % 60;
    int hours = seconds / 3600;

    std::ostringstream formatted;
    formatted 
        << std::setfill('0') << std::setw(2) << hours << ":" 
        << std::setfill('0') << std::setw(2) << mins << ":" 
        << std::setfill('0') << std::setw(2) << secs;

    return formatted.str();

    // NOTE: when running above on WSL, the output of secs looks something like 00:02:081699286144 rather than 00:02:08
    // doing old school C I still get the same problem
    //char buffer[12];
    //snprintf(buffer, sizeof(buffer), "%05d:%02d:%02d", hours, mins, secs);
    //return std::string(buffer);
}