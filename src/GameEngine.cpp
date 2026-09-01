#include "GameEngine.hpp"
#include <iostream>

GameEngine::GameEngine() { m_logger.debugMode = m_debugMode; }

void GameEngine::loadConfigs(const std::string &configFile) {
  m_logger.info("Reading " + configFile);
}

void GameEngine::loadAssets(const std::string &configFile) {
  m_assets.loadConfigs(configFile);
}


void GameEngine::setDebugMode(bool value) {
  if (m_debugMode && value) { return; }
  m_debugMode = value;
  m_logger.debugMode = value;
  if (m_debugMode)
    m_logger.info("ENABLING DEBUG MDOE");
}

void GameEngine::init() {
  // TODO
}

void GameEngine::run() {
  std::cout << "todo" << std::endl;
}
