#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <chrono>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

const std::string kProcDirectory{"/proc/"};
const std::string kStatCmdline{"/cmdline"};
const std::string kStatus{"/status"};
const std::string kUsers{"/etc/passwd"};
const std::string kStat{"/stat"};

int Process::Pid() {
    return pid_;
}
 
float Process::CpuUtilization() {
    string discard, uTimeRaw, sTimeRaw, cuTimeRaw, csTimeRaw;
    string line;
    std::ifstream stream(kProcDirectory + to_string(pid_) + kStat);

    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);

        for (int i = 0; i < 14; ++i) { linestream >> discard; }

        linestream >> uTimeRaw >> sTimeRaw >> cuTimeRaw >> csTimeRaw;

        stream.close();
    }

    long uTime = stof(uTimeRaw);
    long sTime = stof(sTimeRaw);
    //long cuTime = stof(cuTimeRaw);
    long csTime = stof(csTimeRaw);
    
    //std::cout << "processed (13-17) times: " << uTime << " " << sTime << " " << cuTime << " " << csTime << " " << std::endl;

    long totalTicks = uTime + sTime + /*cuTime +*/ csTime;
    //std::cout << "total ticks: " << totalTicks << std::endl;

    long upTimeProcSecs = UpTime();
    //std::cout << "uptime proc secs: " << upTimeProcSecs << std::endl;

    if (totalTicks == 0 || upTimeProcSecs == 0)
        return 99.99f;

    auto cpuUsage = 100 * ((totalTicks / sysconf(_SC_CLK_TCK)) / (float)upTimeProcSecs);

    //std::cout << "total ticks DIV'd herz / uptimeprocsecs: " << 100 * ((totalTicks / sysconf(_SC_CLK_TCK)) / (float)upTimeProcSecs) << std::endl;

    //std::cout << "cpu usage: " << cpuUsage << std::endl;

    return cpuUsage;
}

string Process::Command() {
    if (command_ == "") {
        string cmd;
        string line;
        std::ifstream stream(kProcDirectory + to_string(pid_) + kStatCmdline);

        if (stream.is_open()) {
            std::getline(stream, line);
            std::istringstream linestream(line);
            linestream >> cmd;
        }
        command_ = cmd;
    }

    return command_;
}

string Process::Ram() const {
    string keyword, rawValue;
    string line;
    
    std::ifstream stream(kProcDirectory + to_string(pid_) + kStatus);
    
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> keyword >> rawValue;

            if (keyword == "VmSize:") {
                int value = stoi(rawValue);

                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << ((value <= 0) ? value : (value / 1000.0f));
                
                return oss.str();
            }
        }
    }

    return "0.00000001";
}

string Process::User() {
    if (userName_ == "") {
        string keyw, user, _, uid;
        string line;

        std::ifstream streamStatus(kProcDirectory + to_string(pid_) + kStatus);

        if (streamStatus.is_open()) {
            while (std::getline(streamStatus, line)) {
                std::istringstream linestream(line);
                linestream >> keyw >> uid;

                if (keyw != "Uid:")
                    continue;

                uid_ = stoi(uid);
            }
        }

        //std::cout << "proc status PROCESSED uid: " << uid_ << std::endl;

        std::ifstream streamUser(kUsers);

        if (streamUser.is_open()) {
            while (std::getline(streamUser, line)) {
                std::replace(line.begin(), line.end(), ':', ' ');
                std::istringstream linestream(line);
                linestream >> user >> _ >> uid;
                //std::cout << "passwd uid: " << uid << " : " << user << std::endl;

                if (uid == std::to_string(uid_))
                    return user;
            }
        }
    }

    return userName_;
}

// NOTE: the value we are pulling for the process starttime NEVER changes, so can be stored after first pull
long int Process::UpTime() {
    auto systemUpTime = LinuxParser::UpTime();

    if (upTime_ == 0) {
        string time;
        string line;
        std::ifstream stream(kProcDirectory + to_string(pid_) + kStat);

        if (stream.is_open()) {
            std::getline(stream, line);
            std::istringstream linestream(line);

            for (int i = 0; i < 21; ++i) { linestream >> time; }

            linestream >> time;
        }

        //std::cout << "unprocessed uptime: " << time << std::endl;
        stream.close();

        upTime_ = stol(time) / sysconf(_SC_CLK_TCK);
        //std::cout << "processed uptime: " << time_up << std::endl;
    }

    return systemUpTime - upTime_;
}

// NOTE: I want this so sort by CPU, but that method would need to be made const, and it cannot be as it calls UpTime(), which definitely can't be const. Trying Ram() as that can be const
bool Process::operator<(Process const& a) const {
    //std::cout << "in op overload. ram: " << a.Ram() << ":" << this->Ram() << std::endl;

    auto aRam = stof(a.Ram()); //std::cout << "in op overload. a_ram: " << aRam << std::endl;
    auto thisRam = stof(this->Ram()); //std::cout << "in op overload. a_ram: " << thisRam << std::endl;

    return aRam < thisRam;
}