#include "Logger.hpp"
#include <iostream>

void Logger::info(const std::string &message) {
  std::cout << m_name << " - ";
  std::cout << message;
  std::cout << std::endl;
}
