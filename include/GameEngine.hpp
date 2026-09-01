/*
  GameEngine.hpp
  Core class to orchestrate the high level functionality
  of the game. We make use of the rest of things here, and
  provide the main run loop.
*/

#pragma once

#include <string>
#include "AssetStore.hpp"

class GameEngine : public Core {
  AssetStore m_assets;

public:
  GameEngine();
  GameEngine(std::shared_ptr<Logger> &logger);
  void loadConfigs(const std::string &configFile);
  void loadAssets(const std::string &configFile);
  void init();
  void run();
};
