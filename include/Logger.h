//
#pragma once
#include <string>
#include <ostream>
#include <iostream>
#include <fstream>

#include <mutex>
#include <map>

class Logger
{

public:
   enum class Level
   {
      FATAL,
      ERROR,
      WARNING,
      DEBUG,
      INFO,
      VERBOSE
   };

   static Logger& getInstance();
   void setOutput(std::ostream& stream);
   void log(Level l, const std::string& msg);
   void setLogLevel(Level l);
   void setLogFile(const std::string& path);
   ~Logger();

private:
   std::map<Level, std::string> prefix_map_{{ Level::FATAL, "FATAL" }, { Level::ERROR, "ERROR" }, { Level::WARNING, "WARNING" }, { Level::VERBOSE, "VERBOSE" }, { Level::DEBUG, "DEBUG" }, { Level::INFO, "INFO" }};
   Logger();

   std::ostream* out_;
   std::mutex mtx_;
   Level minLoglevel_;
   std::ofstream logFile_;
};

namespace LogLevel
{
inline constexpr auto FATAL = Logger::Level::FATAL;
inline constexpr auto ERROR = Logger::Level::ERROR;
inline constexpr auto WARNING = Logger::Level::WARNING;
inline constexpr auto DEBUG = Logger::Level::DEBUG;
inline constexpr auto INFO = Logger::Level::INFO;
inline constexpr auto VERBOSE = Logger::Level::VERBOSE;

}

inline void Log(Logger::Level l, std::string msg)
{
   Logger::getInstance().log(l,msg);
}
