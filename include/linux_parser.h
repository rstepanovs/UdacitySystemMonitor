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
   std::string fname{""};
   char state;
   int ppid;
   int pgrp;
   int session;
   int tty_nr;
   int tpgid;
   unsigned flags;
   unsigned long minflt;
   unsigned long cminflt;
   unsigned long majflt;
   unsigned long cmajflt;
   unsigned long utime;
   unsigned long stime;
   long cutime;
   long cstime;
   long priority;
   long nice;
   long num_threads;
   long itrealvalue;
   unsigned long long starttime;
   unsigned long vsize;
   long rss;
   unsigned long rsslim;
   unsigned long startcode;
   unsigned long endcode;
   unsigned long startstack;
   unsigned long kstkesp;
   unsigned long kstkeip;
   unsigned long signal;
   unsigned long blocked;
   unsigned long sigignore;
   unsigned long sigcatch;
   unsigned long wchan;
   unsigned long nswap;
   unsigned long cnswap;
   int exit_signal;
   int processor;
   unsigned rt_priority;
   unsigned policy;
   unsigned long long delayacct_blkio_ticks;
   unsigned long guest_time;
   long cguest_time;
   unsigned long start_data;
   unsigned long end_data;
   unsigned long start_brk;
   unsigned long arg_start;
   unsigned long arg_end;
   unsigned long env_start;
   unsigned long env_end;
   int exit_code;
};


ProcStat GetProcStat(int pid);

};  // namespace LinuxParser




#endif