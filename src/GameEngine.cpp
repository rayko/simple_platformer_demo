#include "GameEngine.hpp"
#include <iostream>

GameEngine::GameEngine() {
  m_logOrigin = "GameEngine";
}

GameEngine::GameEngine(std::shared_ptr<Logger> &logger) {
  m_logOrigin = "GameEngine";
  m_logger = logger;
  m_assets = AssetStore(logger);
}

void GameEngine::loadConfigs(const std::string &configFile) {
  logInfo("Reading " + configFile);
}

void GameEngine::loadAssets(const std::string &configFile) {
  m_assets.loadConfigs(configFile);
}

void GameEngine::init() {
  // TODO
}

void GameEngine::run() {
  std::cout << "todo" << std::endl;
}
