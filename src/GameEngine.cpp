#include <SFML/Graphics.hpp>
#include "GameEngine.hpp"
#include "scenes/Menu.hpp"

using Scenes::Menu;

GameEngine::GameEngine() { m_logOrigin = "GameEngine"; }

GameEngine::GameEngine(std::shared_ptr<Logger> &logger) {
  m_logOrigin = "GameEngine";
  m_logger = logger;
  m_assets = AssetStore(logger);
  init();
}

bool GameEngine::running() { return m_running && m_window.isOpen(); }
sf::RenderWindow & GameEngine::window() { return m_window; }

void GameEngine::run() {
  while (running()) {
    update();
  }
}



// Private

void GameEngine::loadConfigs() {
  logInfo("Loading " + m_engineConfigFile);
}

void GameEngine::loadAssets() {
  logInfo("Loading " + m_assetsConfigFile);
  m_assets.loadConfigs(m_assetsConfigFile);
}

void GameEngine::init() {
  loadConfigs();
  loadAssets();
  m_window.create(sf::VideoMode(m_winSize), m_winTitle);
  m_window.setFramerateLimit(60);
  m_window.setKeyRepeatEnabled(false);

  // Switch to the first scene to load
  changeScene("MainMenu", std::make_shared<Scenes::Menu>(this));
}

std::shared_ptr<Scenes::Base> GameEngine::currentScene() {
  return m_sceneMap[m_currentSceneName];
}

void GameEngine::changeScene(const std::string &name, std::shared_ptr<Scenes::Base> scene) {
  logDebug("Loading scene " + name);
  m_currentSceneName = name;
  m_sceneMap[name] = scene;
  // m_sceneMap[name]->init();
  // ??
}

void GameEngine::update() {
  // TODO
}

void GameEngine::quit() { m_running = false; }

void GameEngine::sUserInput() {
  while (const std::optional event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) { quit(); }
    const ActionMap actions = currentScene()->getActionMap();
    if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
      if (key->scancode == sf::Keyboard::Scancode::X) {
        // Special key, we handle this one here
        takeScreenshot();
      } else {
        if (actions.find((int) key->scancode) == actions.end()) { continue; }
        const Action action = Action(actions.at((int)key->scancode), Action::State::Start);
        currentScene()->doAction(action);
      }
    } else if (const auto *key = event->getIf<sf::Event::KeyReleased>()) {
      if (actions.find((int) key->scancode) == actions.end()) { continue; }
      const Action action = Action(actions.at((int)key->scancode), Action::State::End);
      currentScene()->doAction(action);
    }
  }
}

void GameEngine::takeScreenshot() {
  sf::Texture tex(m_window.getSize());
  tex.update(m_window);
  if (tex.copyToImage().saveToFile("output.png"))
    logDebug("Took screenshot output.png");
}
