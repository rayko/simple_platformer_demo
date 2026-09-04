/*
  GameEngine.hpp
  Core class to orchestrate the high level functionality
  of the game. We make use of the rest of things here, and
  provide the main run loop.
*/

#pragma once

#include <string>
#include "AssetStore.hpp"
#include "Scene.hpp"

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine : public Core {
  sf::RenderWindow m_window;
  AssetStore m_assets;
  std::string m_currentScene;
  SceneMap m_sceneMap;
  size_t m_simulationSpeed = 1;
  bool m_running = true;

  void init(const std::string &configFile);
  void update();
  void sUserInput();
  std::shared_ptr<Scene> currentScene();

public:
  GameEngine();
  GameEngine(std::shared_ptr<Logger> &logger);
  void loadConfigs(const std::string &configFile);
  void loadAssets(const std::string &configFile);
  void init();
  void run();

  sf::RenderWindow window();
  const AssetStore &assetStore() const;
  bool running();
};
