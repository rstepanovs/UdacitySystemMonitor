#include <vector>
#include "processor.h"
#include "Logger.h"
#include "linux_parser.h"


Processor::Processor() : prevWorkTime_(0), prevIdleTime_(0) {}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

   std::vector<std::string> cpuTime(LinuxParser::CpuUtilization());

/*
user: normal processes executing in user mode

nice: niced processes executing in user mode

system: processes executing in kernel mode

idle: twiddling thumbs

iowait: In a word, iowait stands for waiting for I/O to complete. But there are several problems:

CPU will not wait for I/O to complete, iowait is the time that a task is waiting for I/O to complete. When CPU goes into idle state for outstanding task I/O, another task will be scheduled on this CPU.
In a multi-core CPU, the task waiting for I/O to complete is not running on any CPU, so the iowait of each CPU is difficult to calculate.
The value of iowait field in /proc/stat will decrease in certain conditions.
So, the iowait is not reliable by reading from /proc/stat.

irq: servicing interrupts

softirq: servicing softirqs

steal: involuntary wait

guest: running a normal guest

guest_nice: running a niced guest



*/

   float user = std::stof(cpuTime[0]);
   float nice = std::stof(cpuTime[1]);
   float system = std::stof(cpuTime[2]);
   float idle = std::stof(cpuTime[3]);
   float iowait = std::stof(cpuTime[4]);
   float irq = std::stof(cpuTime[5]);
   float softIrq = std::stof(cpuTime[6]);
   float steal = std::stof(cpuTime[7]);
   float guest = std::stof(cpuTime[8]);
   float niceGuest = std::stof(cpuTime[9]);

   float workTime = user + nice + system + irq + softIrq + steal + guest + niceGuest;
   float idleTime = idle + iowait;
   float dWorkTime = workTime - prevWorkTime_;
   float dIdleTime = idleTime - prevIdleTime_;
   prevWorkTime_ = workTime;
   prevIdleTime_ = idleTime;

   return dWorkTime / (dWorkTime + dIdleTime);
}