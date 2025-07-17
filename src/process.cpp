#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid), cmdline_(LinuxParser::Command(pid)),
                            uid_(LinuxParser::Uid(pid)),
                            user_(LinuxParser::GetUserById(uid_)),
                            stat_(LinuxParser::GetProcStat(pid)){}


int Process::Pid() { return pid_; }

float Process::CpuUtilization()  const{
   float totalTimeSec = (stat_.utime + stat_.stime) / sysconf(_SC_CLK_TCK);
   float cpu_usage = totalTimeSec  / UpTime();

   return cpu_usage;
}

string Process::Command() {
   if (cmdline_ != "")
      return cmdline_;
   else
      return stat_.fname;
}

string Process::Ram() {

   auto ram = LinuxParser::Ram(pid_);

   //Expected memory in MB, so we don't need the code below

/*
   string suffix(" kB");

   if (ram/1024)
   {
      ram = ram / 1024;
      suffix = " Mb";
   }

   if (ram/1024)
   {
      ram = ram / 1024;
      suffix = " Gb";
   }


return std::to_string(ram) + suffix;
*/

   return std::to_string(ram / 1024);
}

string Process::User() { return user_; }

long int Process::UpTime() const{
   long systemUpTime = LinuxParser::UpTime();
   long secondsAfterBoot = stat_.starttime / sysconf(_SC_CLK_TCK);
   return systemUpTime - secondsAfterBoot;
}

bool Process::operator<(Process const& a) const {

   return (this->CpuUtilization() < a.CpuUtilization());
}
bool Process::operator>(Process const& a) const {

   return (this->CpuUtilization() > a.CpuUtilization());
}


