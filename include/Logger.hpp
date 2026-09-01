/*
  Logger.hpp
  Simple logger to output stuff to console.
*/

#pragma once
#include <string>

namespace LOGGER {
  enum LEVEL {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
  };
}

class Logger {
  LOGGER::LEVEL level = LOGGER::INFO;

  void emmit(const std::string &message);
public:
  Logger() {};
  void setLevel(LOGGER::LEVEL newLevel);
  void log(LOGGER::LEVEL level, const std::string &message);
  void info(const std::string &message);
  void debug(const std::string &message);
  void warn(const std::string &message);
  void error(const std::string &message);
};
