/*
  GameEngine.hpp
  Core class to orchestrate the high level functionality
  of the game. We make use of the rest of things here, and
  provide the main run loop.
*/

#pragma once

#include <string>

#include "Logger.hpp"
#include "AssetStore.hpp"

class GameEngine {
  Logger m_logger = Logger("GameEngine");
  bool m_debugMode = true;

  AssetStore m_assets;

public:
  GameEngine();
  void loadConfigs(const std::string &configFile);
  void loadAssets(const std::string &configFile);
  void setDebugMode(bool value);
  void init();
  void run();
};
