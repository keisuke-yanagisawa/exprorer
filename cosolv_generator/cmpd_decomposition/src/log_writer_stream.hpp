#include <iostream>
#include <fstream>
#include <time.h>
#include <boost/format.hpp>

#ifndef LOG_WRITER_STREAM_H_
#define LOG_WRITER_STREAM_H_
namespace logs{
  struct LogType {
    std::string type;
    LogType() {}
    LogType(const std::string& type) : type(type) {}
  };
  extern std::ofstream lout;
  extern LogType info;
  extern LogType debug;
  extern LogType warn;
  extern LogType error;
  std::ostream& operator<<(std::ostream& os, const LogType& lt);
}

#endif
