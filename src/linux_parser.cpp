#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <filesystem>
#include "Logger.h"


#include "linux_parser.h"

namespace LinuxParser
{

using std::stof;
using std::string;
using std::to_string;
using std::vector;


struct ProcStat GetProcStat(int pid)
{
   struct ProcStat stat = { 0 };
   string path = kProcDirectory + "/" + std::to_string(pid) + kStatFilename;
   std::ifstream filestream(path);
   if (filestream)
   {
      string line;
      std::getline(filestream, line);
      std::istringstream lineStream(line);
      lineStream >> stat.pid;
      size_t startFname = line.find('(');
      size_t endFname = line.rfind(')');
      if (startFname == std::string::npos || endFname == std::string::npos || endFname <= startFname) return stat;
      stat.fname = line.substr(startFname + 1, endFname - startFname - 1);
      std::string rest = line.substr(endFname + 2);
      std::istringstream ss(rest);
      ss >> stat.state >> stat.ppid >> stat.pgrp >> stat.session >> stat.tty_nr >> stat.tpgid
       >> stat.flags >> stat.minflt >> stat.cminflt >> stat.majflt >> stat.cmajflt
       >> stat.utime >> stat.stime >> stat.cutime >> stat.cstime >> stat.priority
       >> stat.nice >> stat.num_threads >> stat.itrealvalue >> stat.starttime
       >> stat.vsize >> stat.rss >> stat.rsslim >> stat.startcode >> stat.endcode
       >> stat.startstack >> stat.kstkesp >> stat.kstkeip >> stat.signal
       >> stat.blocked >> stat.sigignore >> stat.sigcatch >> stat.wchan
       >> stat.nswap >> stat.cnswap >> stat.exit_signal >> stat.processor
       >> stat.rt_priority >> stat.policy >> stat.delayacct_blkio_ticks
       >> stat.guest_time >> stat.cguest_time >> stat.start_data >> stat.end_data
       >> stat.start_brk >> stat.arg_start >> stat.arg_end >> stat.env_start
       >> stat.env_end >> stat.exit_code;
   }

   return stat;
}

// Read file content into string
string getFileContent(std::string path)
{
   std::ifstream filestream(path);
   string content;

   if (filestream)
      {
         std::stringstream buf;
         buf << filestream.rdbuf();
         content = buf.str();
   }
   else
   {
      Log(LogLevel::ERROR, "Failed to open " + path);
   }

   return content;
}

// Find regex value
string getValue(string content, string pat)
{
   string value;
   std::regex pattern(pat, std::regex::extended);
   std::smatch match;
   if (std::regex_search(content, match, pattern))
   {
      value = match[1].str();
   }
   else
   {
      Log(LogLevel::DEBUG, "pattern \"" + pat + "\" not found");
   }
   return value;
}


// Get tuple of matches by pattern from content
std::tuple<string, string> getTuple(string content, string p)
{
   std::regex pattern(p, std::regex::extended);
   std::smatch match;
   std::tuple<string, string> results;
   if (std::regex_search(content, match, pattern) && match.size() == 3)
   {
      results = {match[1].str(), match[2].str()};
   } else
   {
      Log(LogLevel::ERROR, "getTuple: Failed to find pattern " + p);
   }
   return results;
}

// get vector of matches from content
std::vector<string> getVector(string content, string p)
{
   std::regex pattern(p, std::regex::extended);
   std::smatch match;
   std::vector<string> results;
   if (std::regex_search(content, match, pattern))
   {
      for (std::size_t i = 1; i < match.size(); i++)
      {
         results.push_back(match[i].str());
      }
   }
   return results;
}

string OperatingSystem()
{
   string content = getFileContent(kOSPath);
   string pattern("PRETTY_NAME[[:space:]]*=[[:space:]]*\"([^\"]+)\"");
   return getValue(content, pattern);
}

// Read /etc/passwd and build a map UID => Username
std::map<string, string> GetUserMap()
{
   std::map<string, string> userMap;
   std::ifstream stream(kPasswordPath);

   if (stream.is_open())
   {
      string line;
      std::regex pattern("([^[:space:]]+):x:([[:digit:]]+):", std::regex::extended);
      while (!stream.eof())
      {
         std::smatch match;
         std::getline(stream, line);
         if (std::regex_search(line, match, pattern))
         {
            userMap.insert(std::pair<string, string>(match[2].str(), match[1].str()));
         }
      }
   }
   else
   {
      Log(LogLevel::ERROR, "Failed to open " + kPasswordPath);
   }

   return userMap;
}

string GetUserById(string uid)
{
   static std::map<string, string> userMap(GetUserMap());
   return userMap[uid];
}

// DONE: An example of how to read data from the filesystem
string Kernel()
{
   string os, kernel, version;
   string line;
   std::ifstream stream(kProcDirectory + kVersionFilename);
   if (stream.is_open())
   {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> os >> version >> kernel;
   }
   else
   {
      Log(LogLevel::ERROR, "Filed to open " + kProcDirectory + kVersionFilename);
   }
   return kernel;
}

vector<int> Pids()
{
   vector<int> pids;

   for (auto &f : std::filesystem::directory_iterator(kProcDirectory))
   {
      if (f.is_directory())
      {
         string filename = f.path().stem().string();
         if (std::all_of(filename.begin(), filename.end(), isdigit))
         {
            int pid = stoi(filename);
            pids.push_back(pid);
         }
      }
   }
   return pids;
}

float MemoryUtilization() {

   string content = getFileContent(kProcDirectory + kMeminfoFilename);
   std::map<std::string, unsigned long> size_map
   {{ "kB", 1024 }, { "M", 1024 * 1024 }, { "G", 1024 * 1024 * 1024 }};

   string totalMemPattern("MemTotal:[[:space:]]*([[:digit:]]+)[[:space:]]*([^[:space:]]+)");
   string freeMemPattern("MemFree:[[:space:]]*([[:digit:]]+)[[:space:]]*([^[:space:]]+)");
   auto [totalMemStr, sizeTotStr] = getTuple(content, totalMemPattern);
   auto [freeMemStr, sizeFreeStr] = getTuple(content, freeMemPattern);

   float totalMem = std::stof(totalMemStr);
   float freeMem = std::stof(freeMemStr);

   float result = (totalMem - freeMem) / totalMem;
   return result;
}

// TODO: Read and return the number of jiffies for the system
long Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long ActiveJiffies(int pid[[maybe_unused]])
{ return 0; }

// TODO: Read and return the number of active jiffies for the system
long ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long IdleJiffies() { return 0; }

vector<string> CpuUtilization() {
   string pattern("cpu[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)[[:space:]]*([[:digit:]]+)");
   string content = getFileContent(kProcDirectory + kStatFilename);
   return getVector(content, pattern);
}

int TotalProcesses() {
   string pattern("processes[[:space:]]*([[:digit:]]+)");
   string content = getFileContent(kProcDirectory + kStatFilename);

   return std::stoi(getValue(content, pattern));
}

int RunningProcesses() {
   string pattern("procs_running[[:space:]]*([[:digit:]]+)");
   string content = getFileContent(kProcDirectory + kStatFilename);

   return std::stoi(getValue(content, pattern));
 }

string Command(int pid)
{
   string line;
   string fname = kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename;
   std::ifstream stream(fname);
   if (stream.is_open())
   {
      std::getline(stream, line, '\0');
   }
   else
   {
      Log(LogLevel::ERROR, "Filed to open " + fname);
   }
   return line;
}

unsigned long Ram(int pid)
{
   string path(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
   string content = getFileContent(path);
   string pattern("VmRSS:[[:space:]]+([[:digit:]]+)[[:space:]]kB");
   string val = getValue(content, pattern);
   //Log(LogLevel::DEBUG, "Got VmRSS value for pid " + std::to_string(pid) + " :"+ val);
   unsigned long ram = (val == "" ? 0 : std::stoul(val));
   return ram;
}

string Uid(int pid)
{
   string fname = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
   string content = getFileContent(fname);
   string pattern = "Uid:[[:space:]]+([[:digit:]]+)";
   return getValue(content, pattern);
}

// TODO: Read and return the system uptime
//long UpTime(int pid[[maybe_unused]]) { return 0; }


long UpTime()
{
   string pattern("([[:digit:]]+.[[:digit:]]+)[[:space:]]*[[:digit:]]");
   string content = getFileContent(kProcDirectory + kUptimeFilename);

   long val = std::stol(getValue(content, pattern));

   //Log(LogLevel::DEBUG, "UpTime return " + std::to_string(val));

   return val;
}

}; //namespace LinuxParser
