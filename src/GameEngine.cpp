#include <SFML/Graphics.hpp>
#include "GameEngine.hpp"
#include "scenes/Menu.hpp"

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

const AssetStore &GameEngine::assetStore() const { return m_assets; }

// Helper to return a number of frames based on input seconds
// just to not have to deal with exact frame numbers on timed things.
size_t GameEngine::seconds(float secs) {
  return std::round(maxFPS * secs);
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
  m_window.setFramerateLimit(maxFPS);
  m_window.setKeyRepeatEnabled(false);

  // Switch to the first scene to load
  changeScene("MainMenu", std::make_shared<Scenes::Menu>(this));
}

std::shared_ptr<Scenes::Base> GameEngine::currentScene() {
  return m_sceneMap[m_currentSceneName];
}

void GameEngine::changeScene(const std::string &name, std::shared_ptr<Scenes::Base> scene) {
  logInfo("Loading scene " + name);
  logDebug("Setting current scene");
  m_currentSceneName = name;
  logDebug("Saving on m_sceneMap");
  m_sceneMap[name] = scene;
}

void GameEngine::update() {
  sUserInput();
  currentScene()->update();
  m_currentFrame++;

  if (currentScene()->isFinished()) {
    // TODO We don't have scenes in order yet, if one exits, we have nothing
    // more to do. Change this

    logDebug("Current scene " + m_currentSceneName + " Finished, no more scenes");
    quit();
  }
}

void GameEngine::quit() { m_running = false; }

void GameEngine::sUserInput() {
  while (const std::optional event = m_window.pollEvent()) {
    if (currentScene()->receivePointerLocation()) {
      sf::Vector2i mousePosition = sf::Mouse::getPosition(window());
      currentScene()->setPointerPos(Vec2f(mousePosition.x, mousePosition.y));
    }

    if (event->is<sf::Event::Closed>()) { quit(); }

    if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
      if (key->scancode == sf::Keyboard::Scancode::X) {
        // Special key, we handle this one here
        takeScreenshot();
      } else {
        handleKeyboardEvent(key->scancode, Action::State::Start);
      }
    } else if (const auto *key = event->getIf<sf::Event::KeyReleased>()) {
      handleKeyboardEvent(key->scancode, Action::State::End);
    }

    if (const auto *btn = event->getIf<sf::Event::MouseButtonPressed>()) {
      handleMouseEvent(btn->button, Action::State::Start);
    } else if (const auto *btn = event->getIf<sf::Event::MouseButtonReleased>()) {
      handleMouseEvent(btn->button, Action::State::End);
    }

    // Wheel is handled differently so we convert scrolling to our
    // custom set of events to send to the scene instead.
    if (const auto *scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
      switch(scroll->wheel) {
        case (sf::Mouse::Wheel::Vertical):
          if (scroll->delta > 0)
            handleMouseWheelEvent(Action::MWheelEvent::ScrollUp);
          else if (scroll->delta < 0)
            handleMouseWheelEvent(Action::MWheelEvent::ScrollDown);
          break;
        case (sf::Mouse::Wheel::Horizontal):
          if (scroll->delta > 0)
            handleMouseWheelEvent(Action::MWheelEvent::ScrollLeft);
          else if (scroll->delta < 0)
            handleMouseWheelEvent(Action::MWheelEvent::ScrollRight);
          break;
        default: break;
      }
    }
  }
}

void GameEngine::handleMouseWheelEvent(Action::MWheelEvent scroll) {
  if (!currentScene()->respondsToMouseWheel(scroll)) { return; }
  const Action action = Action(currentScene()->mouseWheelMap().at(scroll), Action::State::Triggered);
  currentScene()->doAction(action);
  logDebug("Mouse: Sent action " + action.str() + " to current scene " + m_currentSceneName);
}

void GameEngine::handleMouseEvent(sf::Mouse::Button btn, Action::State state) {
  if (!currentScene()->respondsToMouseBtn(btn)) { return; }
  const Action action = Action(currentScene()->mouseMap().at(btn), state);
  currentScene()->doAction(action);
  logDebug("Mouse: Sent action " + action.str() + " to current scene " + m_currentSceneName);
}

void GameEngine::handleKeyboardEvent(sf::Keyboard::Scancode key, Action::State state) {
  if (!currentScene()->respondsToKey(key)) { return; }
  const Action action = Action(currentScene()->keyMap().at(key), state);
  currentScene()->doAction(action);
  logDebug("Keyboard: Sent action " + action.str() + " to current scene " + m_currentSceneName);
}


void GameEngine::takeScreenshot() {
  sf::Texture tex(m_window.getSize());
  tex.update(m_window);
  if (tex.copyToImage().saveToFile("output.png"))
    logDebug("Took screenshot output.png");
}
