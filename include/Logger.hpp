/*
  Logger.hpp
  A simple logger helper to output information to console.
*/

#pragma once

#include <string>

class Logger {
  const std::string m_name = "default";

public:
  bool debugMode = true; // Enable or disable debug output

  Logger(const std::string &name) : m_name(name) {}

  void info(const std::string &message);
  void debug(const std::string &message);
  void error(const std::string &message);
};
