#include "Logger.hpp"
#include <iostream>

void Logger::log(Logger::Level level, const std::string &message) {
  switch(level){
  case (Logger::Level::Debug):
    debug(message);
    break;
  case (Logger::Level::Info):
    info(message);
    break;
  case (Logger::Level::Warn):
    warn(message);
    break;
  case (Logger::Level::Error):
    error(message);
    break;
  default:
    break;
  }
}

void Logger::emmit(const std::string &message){
  std::cout << message << std::endl;
}

void Logger::setLevel(Logger::Level newLevel) {
  level = newLevel;
}

void Logger::debug(const std::string &message) {
  if (level > Logger::Level::Debug) { return; }
  emmit("DEBUG - " + message);
}

void Logger::info(const std::string &message) {
  if (level > Logger::Level::Info) { return; }
  emmit("INFO - " + message);
}

void Logger::warn(const std::string &message) {
  if (level > Logger::Level::Warn) { return; }
  emmit("WARN - " + message);
}

void Logger::error(const std::string &message) {
  if (level > Logger::Level::Error) { return; }
  emmit("ERROR - " + message);
}
