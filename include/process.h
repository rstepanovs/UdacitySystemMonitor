#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime() const;
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
    int pid_;
    std::string cmdline_;
    std::string uid_;
    std::string user_;
    LinuxParser::ProcStat stat_;
};

#endif