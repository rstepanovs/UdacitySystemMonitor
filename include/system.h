#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
   ~System();
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string systemName_;
  std::string kernelVersion_;
  std::vector<int> prevPids_;
  std::vector<int> GetRemovedPids(std::vector<int>& oldPids, std::vector<int>& newPids);
  std::vector<int> GetAddedPids(std::vector<int>& oldPids, std::vector<int>& newPids);
  void RefreshProcesses();
};

#endif