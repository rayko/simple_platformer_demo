/*
  Core.hpp
  Basic commons for major classes like GameEngine, EntityManager
  and AssetStore. Provides some basic functionality shared across
  these other classes, like logging, debug flags and helpers.
*/

#pragma once

#include "Logger.hpp"
#include <memory>

class Core {
protected:
  std::shared_ptr<Logger> m_logger; // Pointer to common logger
  bool m_debugMode = false; // Emmit or not debug messages to log
  std::string m_logOrigin = "Core";

  // Logging helpers
  void logDebug(const std::string &message);
  void logInfo(const std::string &message);
  void logWarn(const std::string &message);
  void logError(const std::string &message);

  void logDebug(const std::string &message) const;
  void logInfo(const std::string &message) const;
  void logWarn(const std::string &message) const;
  void logError(const std::string &message) const;

  // Simple crash with message helper
  void exitWithError(const std::string &message);
  void exitWithError(const std::string &message, int code);

  void exitWithError(const std::string &message) const;
  void exitWithError(const std::string &message, int code) const;

  // Common file opening with auto-exit
  std::ifstream openFile(const std::string &path);

public:
  Core() {};
  void enableDebug();
  void disableDebug();
  const std::string logOrigin() const;
  void setLogger(std::shared_ptr<Logger> &logger);
};
