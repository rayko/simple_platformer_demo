/*
  Logger.hpp
  Simple logger to output stuff to console.
*/

#pragma once
#include <string>

class Logger {
public:
  enum class Level { Debug, Info, Warn, Error };

  Logger() {};
  void setLevel(Level newLevel);
  void log(Level level, const std::string &message);
  void info(const std::string &message);
  void debug(const std::string &message);
  void warn(const std::string &message);
  void error(const std::string &message);

private:
  // This is below here because I need LEVEL to be
  // defined first so I can set the private level.
  Level level = Level::Info;

  void emmit(const std::string &message);
};
