#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

#include "Logger.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System() : systemName_(LinuxParser::OperatingSystem()),
    kernelVersion_(LinuxParser::Kernel()) {
   RefreshProcesses();
}

System::~System() {
  processes_.clear();
}

Processor& System::Cpu() { return cpu_; }

std::vector<int> System::GetRemovedPids(std::vector<int>& oldPids, std::vector<int>& newPids) {
  std::unordered_set<int> newSet(newPids.begin(), newPids.end());
  std::vector<int> removed;

  for (int pid: oldPids) {
    if (newSet.find(pid) == newSet.end()) {
      removed.push_back(pid);
    }
  }
  return removed;
}

std::vector<int> System::GetAddedPids(std::vector<int>& oldPids, std::vector<int>& newPids) {
  std::unordered_set<int> oldSet(oldPids.begin(), oldPids.end());
  std::vector<int> added;
  for (int pid: newPids) {
    if (oldSet.find(pid) == oldSet.end()) {
      added.push_back(pid);
    }
  }

  return added;
}

void System::RefreshProcesses()
{
   std::vector<int> newPids = LinuxParser::Pids();

   // Remove processes if pids gone
   std::vector<int> removedPids = GetRemovedPids(prevPids_, newPids);
   if (!removedPids.empty()) {
     processes_.erase(
         std ::remove_if(processes_.begin(),
                         processes_.end(),
         [&](Process& p) {
         return std::find(removedPids.begin(), removedPids.end(), p.Pid()) != removedPids.end();
     }));
   }

   // Add new processes
  std::vector<int> addedPids = GetAddedPids(prevPids_, newPids);
  if (!addedPids.empty()) {

    for (int pid: addedPids) {
      processes_.push_back(Process(pid));
    }
  }

   prevPids_ = newPids;
   std::sort(processes_.begin(), processes_.end(), std::greater<>());
}

vector<Process>& System::Processes() {
   RefreshProcesses();
  return processes_;
}

float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

std::string System::Kernel() { return kernelVersion_; }

std::string System::OperatingSystem() { return systemName_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses();}

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() {
  long int val = LinuxParser::UpTime();
  Log(LogLevel::DEBUG, "System::UpTime called " + std::to_string(val));
  return val;
}
