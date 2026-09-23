/*
  GameEngine.hpp
  Core class to orchestrate the high level functionality
  of the game. We make use of the rest of things here, and
  provide the main run loop.
*/

#pragma once

#include <string>
#include "AssetStore.hpp"
#include "scenes/Base.hpp"

typedef std::map<std::string, std::shared_ptr<Scenes::Base>> SceneMap;

class GameEngine : public Core {
  sf::RenderWindow m_window;
  AssetStore m_assets;
  std::string m_currentSceneName;
  SceneMap m_sceneMap;
  size_t m_simulationSpeed = 1;
  bool m_running = true;
  size_t m_currentFrame = 0;

  // Not the best place for these, but since this is kinda
  // a singleton, it's ok
  const sf::Vector2u m_winSize = {1280, 768};
  const std::string m_winTitle = "Simple Platformer Demo";
  const std::string m_engineConfigFile = "configs.txt";
  const std::string m_assetsConfigFile = "assets.txt";

  void init();
  void loadConfigs();
  void loadAssets();
  void update();
  void sUserInput();
  std::shared_ptr<Scenes::Base> currentScene();
  void quit();
  void takeScreenshot();

  void handleKeyboardEvent(sf::Keyboard::Scancode key, Action::State state);
  void handleMouseEvent(sf::Mouse::Button btn, Action::State state);
  void handleMouseWheelEvent(Action::MWheelEvent scroll);

public:
  GameEngine();
  GameEngine(std::shared_ptr<Logger> &logger);
  void run();
  void changeScene(const std::string &name, std::shared_ptr<Scenes::Base> scene);

  sf::RenderWindow &window();
  const AssetStore &assetStore() const;
  bool running();
};
