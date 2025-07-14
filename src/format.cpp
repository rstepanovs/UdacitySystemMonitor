#include <sstream>
#include <iomanip>
#include <string>

#include "format.h"
#include "Logger.h"

using std::string;

string Format::ElapsedTime(long s)
{

   int h = s/3600;
   s = s % 3600;
   int m = s / 60;
   s = s % 60;

   std::stringstream ss;

   ss << std::setw(2) << std::setfill('0') << h << ":"
   << std::setw(2) << std::setfill('0') << m << ":"
   << std::setw(2) << std::setfill('0') << s;


   return ss.str();
}