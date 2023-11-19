#include "ncurses_display.h"
#include "system.h"
#include <iostream>

int main() {
  System system;
  NCursesDisplay::Display(system);
  /*
  auto p = system.Processes();
  for(auto _p : p) {
    std::cout << "PID: " << _p.Pid() << std::endl;
    std::cout << "CMD: " << _p.Command() << std::endl;
    std::cout << "RAM: " << _p.Ram() << std::endl;
    std::cout << "USR: " << _p.User() << std::endl;
    std::cout << "UPT: " << _p.UpTime() << std::endl;
    std::cout << "CPU: " << _p.CpuUtilization() << std::endl;
    std::cout << std::endl;
  }
  std::cout << "Num Processes: " << system.Processes().size() << std::endl;
*/
/*
  Process p1 = p[0];
  Process p2 = p[1];
  Process p3 = p[2];

  //std::cout << "p1 > p2: " << p1 > p2 << std::endl;
  auto x1 = &p1 > &p2;
  auto x2 = &p2 > &p1;
  auto x3 = &p3 > &p2;
  std::cout << "p1 > p2: " << x1 << std::endl;
  std::cout << "p2 > p1: " << x2 << std::endl;
  std::cout << "p3 > p2: " << x3 << std::endl;*/
}