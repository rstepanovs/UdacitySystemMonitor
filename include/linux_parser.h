#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#pragma once

#include <fstream>
#include <regex>
#include <string>

#include "Logger.h"

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
unsigned long Ram(int pid);
std::string Uid(int pid);
std::string GetUserById(std::string uid);

struct ProcStat
{
   int pid;
   std::string fname = "";
   char state = ' ';
   int ppid = 0;
   int pgrp = 0;
   int session = 0;
   int tty_nr = 0;
   int tpgid = 0;
   unsigned flags = 0;
   unsigned long minflt = 0UL;
   unsigned long cminflt = 0UL;
   unsigned long majflt = 0UL;
   unsigned long cmajflt = 0UL;
   unsigned long utime = 0UL;
   unsigned long stime = 0UL;
   long cutime = 0L;
   long cstime = 0L;
   long priority = 0L;
   long nice = 0L;
   long num_threads = 0L;
   long itrealvalue = 0L;
   unsigned long long starttime = 0ULL;
   unsigned long vsize = 0UL;
   long rss = 0L;
   unsigned long rsslim = 0UL;
   unsigned long startcode = 0UL;
   unsigned long endcode = 0UL;
   unsigned long startstack = 0UL;
   unsigned long kstkesp = 0UL;
   unsigned long kstkeip = 0UL;
   unsigned long signal = 0UL;
   unsigned long blocked = 0UL;
   unsigned long sigignore = 0UL;
   unsigned long sigcatch = 0UL;
   unsigned long wchan = 0UL;
   unsigned long nswap = 0UL;
   unsigned long cnswap = 0UL;
   int exit_signal = 0;
   int processor = 0;
   unsigned rt_priority = 0;
   unsigned policy = 0;
   unsigned long long delayacct_blkio_ticks = 0ULL;
   unsigned long guest_time = 0UL;
   long cguest_time = 0L;
   unsigned long start_data = 0UL;
   unsigned long end_data = 0UL;
   unsigned long start_brk = 0UL;
   unsigned long arg_start = 0UL;
   unsigned long arg_end = 0UL;
   unsigned long env_start = 0UL;
   unsigned long env_end = 0UL;
   int exit_code = 0;
};


ProcStat GetProcStat(int pid);

};  // namespace LinuxParser




#endif