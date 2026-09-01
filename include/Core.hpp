/*
  Core.hpp
  Basic commons for major classes like GameEngine, EntityManager
  and AssetStore. Provides some basic functionality shared across
  these other classes, like logging, debug flags and helpers.
*/


#include "Logger.hpp"
#include <memory>

class Core {
  std::shared_ptr<Logger> m_logger; // Pointer to common logger
  bool m_debugMode = false; // Emmit or not debug messages to log
  const std::string m_logOrigin = "Core";

  // Logging helpers
  void logDebug(const std::string &message);
  void logInfo(const std::string &message);
  void logWarn(const std::string &message);
  void logError(const std::string &message);

  // Simple crash with message helper
  void exitWithError(const std::string &message, int code);

  // Common file opening with auto-exit
  std::ifstream openFile(const std::string &path);

public:
  Core() {};
  void enableDebug();
  void disableDebug();
  const std::string logOrigin() const;
};
