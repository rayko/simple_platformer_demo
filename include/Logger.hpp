/*
  Logger.hpp
  A simple logger helper to output information to console.
*/

#pragma once

#include <string>

class Logger {
  const std::string m_name = "default";

public:
  Logger(const std::string &name) : m_name(name) {}

  void info(const std::string & message);
};
