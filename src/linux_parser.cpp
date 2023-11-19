#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }

  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        //std::cout << "all-digit dir/pid: " << filename << std::endl;
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string keyword, rawValue;
  string line;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    long total = 0;
    long free = 0;

    while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> keyword >> rawValue;

        if (keyword == "MemTotal:")
          total = stol(rawValue);
        
        if (keyword == "MemFree:")
          free = stol(rawValue);
        
        if (total != 0 && free != 0)
          return (float)free / (float)total;
    }
  }

  return 0.0;
}

long LinuxParser::UpTime() {
  string secs;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> secs;
  }

  return stol(secs);
}

int LinuxParser::TotalProcesses() {
  auto p = Pids();

  return p.size();
}

int LinuxParser::RunningProcesses() {
  string keyword, rawValue;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
      while (std::getline(stream, line)) {
          std::istringstream linestream(line);
          linestream >> keyword >> rawValue;

          if (keyword == "procs_running")
              return stoi(rawValue);
      }
  }

  return 0;
}
