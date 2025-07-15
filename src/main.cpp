#include <string>
#include "ncurses_display.h"
#include "system.h"
#include "Logger.h"


const std::string logFilename = "Log.txt";

int main() {
  Logger& logger = Logger::getInstance();
  logger.setLogLevel(LogLevel::DEBUG);
  logger.setLogFile(logFilename);

  System system;

  NCursesDisplay::Display(system);
}