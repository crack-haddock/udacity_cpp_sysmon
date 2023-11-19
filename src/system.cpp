#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

Processor& System::Cpu() {
    return cpu_;
}

vector<Process>& System::Processes() {
    auto latestProcesses = LinuxParser::Pids();
    //std::cout << "num pids: " << latestProcesses.size() << std::endl;

    //if (processes_.size > 0)
    for (auto _ : processes_) {
        processes_.pop_back();
    }

    for (int pid : latestProcesses) {
        Process p(pid); // creating the process will auto-populate cmdline etc.

        processes_.push_back(p);
    }

    std::sort(processes_.begin(), processes_.end());

    return processes_;
}

std::string System::Kernel() {
    return LinuxParser::Kernel();
}

std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() { 
    return LinuxParser::UpTime();
}
