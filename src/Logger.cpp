#include "Logger.hpp"
#include <iostream>

void Logger::info(const std::string &message) {
  std::cout << "INFO - " + m_name << " - ";
  std::cout << message;
  std::cout << std::endl;
}

void Logger::debug(const std::string &message) {
  // TODO check if we should print debug
  std::cout << "DEBUG - " + m_name << " - ";
  std::cout << message;
  std::cout << std::endl;
}

void Logger::error(const std::string &message) {
  std::cerr << "ERROR - " << m_name << " - ";
  std::cerr << message;
  std::cerr << std::endl;
}
