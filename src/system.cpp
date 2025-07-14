#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

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
                   kernelVersion_(LinuxParser::Kernel()) { }

System::~System()
{
   processes_.clear();
}

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
   processes_.clear();
   vector<int> Pids = LinuxParser::Pids();
   for (std::vector<int>::iterator it = Pids.begin(); it != Pids.end(); ++it)
   {
      processes_.push_back(Process(*it));
   }

   std::sort(processes_.begin(), processes_.end(), std::greater<>());

   return processes_; }

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
