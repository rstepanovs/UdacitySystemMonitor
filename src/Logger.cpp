#include <cassert>

#include "Logger.h"

Logger& Logger::getInstance()
{
   static Logger instance;
   return instance;
}

Logger::Logger() : out_(&std::cout), minLoglevel_(Level::WARNING)
{
}


Logger::~Logger()
{
   logFile_.close();
}

void Logger::setOutput(std::ostream& stream)
{
   std::lock_guard<std::mutex> lock(mtx_);
   out_ = &stream;

}

void Logger::log(Logger::Level l, const std::string& msg)
{
   if (l > minLoglevel_) return;

   std::lock_guard<std::mutex> lock(mtx_);
   (*out_) << "[" << prefix_map_[l] << "] " << msg << std::endl << std::flush;
}


void Logger::setLogFile(const std::string& path)
{
   logFile_.open(path, std::ofstream::out);
   assert(logFile_.is_open());
   setOutput(logFile_);

}

void Logger::setLogLevel(Level l)
{
   minLoglevel_ = l;
}

