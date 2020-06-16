#include "log_writer_stream.hpp"


namespace {
  std::string getDateString() {
    time_t timer = time(NULL);
    tm* date = localtime(&timer);
    std::string dateStr = (boost::format("[%02d/%02d %02d:%02d:%02d]") 
			   % date->tm_mon % date->tm_mday
			   % date->tm_hour % date->tm_min % date-> tm_sec).str();
    return dateStr;
  }
}

namespace logs{
  std::ofstream lout;
  LogType info("INFO");
  LogType debug("DEBUG");
  LogType warn("WARN");
  LogType error("ERROR");
  std::ostream& operator<<(std::ostream& os, const LogType& lt) {
    os << std::endl << getDateString() << " [time=(" << time(NULL) << ")] [" << lt.type << "] ";
    return os;
  }
}
