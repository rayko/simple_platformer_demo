#include "Logger.hpp"
#include <iostream>

void Logger::log(LOGGER::LEVEL level, const std::string &message) {
  switch(level){
  case (LOGGER::DEBUG):
    debug(message);
    break;
  case (LOGGER::INFO):
    info(message);
    break;
  case (LOGGER::WARN):
    warn(message);
    break;
  case (LOGGER::ERROR):
    error(message);
    break;
  default:
    break;
  }
}

void Logger::emmit(const std::string &message){
  std::cout << message << std::endl;
}

void Logger::setLevel(LOGGER::LEVEL newLevel) {
  level = newLevel;
}

void Logger::debug(const std::string &message) {
  if (level > LOGGER::DEBUG) { return; }
  emmit("DEBUG - " + message);
}

void Logger::info(const std::string &message) {
  if (level > LOGGER::INFO) { return; }
  emmit("INFO - " + message);
}

void Logger::warn(const std::string &message) {
  if (level > LOGGER::WARN) { return; }
  emmit("WARN - " + message);
}

void Logger::error(const std::string &message) {
  if (level > LOGGER::ERROR) { return; }
  emmit("ERROR - " + message);
}
