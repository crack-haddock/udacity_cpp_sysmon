#include "processor.h"
#include <iostream>
#include <sstream>
#include <fstream>

using std::string;

const std::string kProcDirectory{"/proc/"};
const std::string kStatFilename{"/stat"};

float Processor::Utilization() {
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
        std::getline(stream, line);
        //std::cout << "raw line: " << line << std::endl;
        std::istringstream linestream(line);
        std::string discard;
        linestream >> discard >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    }

    long grand_total_no_idle = user + nice + system + iowait + irq + softirq + steal + guest + guest_nice;
    long grand_total_with_idle = grand_total_no_idle + idle;
/*
    std::cout << "all tokens: " << user << " " << nice << " " << system << " " << idle << " " << std::endl;
    std::cout << "processor total (no idle: with idle): " << grand_total_no_idle << ":" << grand_total_with_idle << std::endl;
    std::cout << "idle: " << user << std::endl;
*/
    return (float)grand_total_no_idle / (float)grand_total_with_idle;
}
