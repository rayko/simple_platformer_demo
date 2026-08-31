#include "GameEngine.hpp"
#include <iostream>

GameEngine::GameEngine() : m_logger("GameEngine") {

}

void GameEngine::init(const std::string &configFile) {
  m_logger.info("Initialized");
  // TODO
}

void GameEngine::run() {
  std::cout << "todo" << std::endl;
}
