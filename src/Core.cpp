#include "Core.hpp"
#include <fstream>

// Public

void Core::enableDebug() {
  if(m_debugMode) { return; }
  m_debugMode = true;
}

void Core::disableDebug() {
  if(!m_debugMode) { return; }
  m_debugMode = false;
}

const std::string Core::logOrigin() const { return m_logOrigin; }


// Private

void Core::logDebug(const std::string &message) {
  if (!m_logger) { return; }
  m_logger->debug(m_logOrigin + " - " + message);
}

void Core::logInfo(const std::string &message) {
  if (!m_logger) { return; }
  m_logger->info(m_logOrigin + " - " + message);
}

void Core::logWarn(const std::string &message) {
  if (!m_logger) { return; }
  m_logger->warn(m_logOrigin + " - " + message);
}

void Core::logError(const std::string &message) {
  if (!m_logger) { return; }
  m_logger->error(m_logOrigin + " - " + message);
}

void Core::exitWithError(const std::string &message, int code = 1) {
  if (m_logger)
    m_logger->error(m_logOrigin + " - " + message);
  exit(code);
}

std::ifstream Core::openFile(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open())
    exitWithError("Could not open file " + path);
  return file;
}
