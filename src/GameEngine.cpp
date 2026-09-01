#include "GameEngine.hpp"
#include <iostream>

GameEngine::GameEngine(std::shared_ptr<Logger> &logger) { m_logger = logger; }


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
