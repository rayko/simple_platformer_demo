#include "scenes/PlayLevel.hpp"
#include "GameEngine.hpp"
#include "scenes/Menu.hpp"

namespace Scenes {
  ///// Public

  PlayLevel::PlayLevel(GameEngine *engine, const std::string &lvlConfigFile)
    : m_levelPath(lvlConfigFile), Base(engine) {

    m_logOrigin = "Scenes::PlayLevel (" + m_levelPath + ")";
    setLogger(engine->getLogger());
    init();
  }

  void PlayLevel::update() {
    sRender();
  }

  void PlayLevel::doAction(const Action &action) { sDoAction(action); }


  ///// Private

  void PlayLevel::init() {
    // TODO
    logInfo("Loading scene");
    m_gridTextFont = m_engine->assetStore().getFont("SimpleFont");

    logDebug("Mapping actions");
    registerKeyboardAction(sf::Keyboard::Scancode::G, Action::Name::ToggleGrid);
    registerKeyboardAction(sf::Keyboard::Scancode::T, Action::Name::ToggleTextures);
    registerKeyboardAction(sf::Keyboard::Scancode::C, Action::Name::ToggleColliders);
    registerKeyboardAction(sf::Keyboard::Scancode::Escape, Action::Name::Escape);
  }

  void PlayLevel::onEnd() {
    // TODO
    m_finished = true;
  }

  void PlayLevel::sDoAction(const Action &action) {
    if (action.starting()) {
      switch(action.name()) {
      case (Action::Name::ToggleGrid):
        m_drawGrid = !m_drawGrid;
        break;
      case (Action::Name::ToggleTextures):
        m_drawTextures = !m_drawTextures;
        break;
      case (Action::Name::ToggleColliders):
        m_drawColliders = !m_drawColliders;
        break;
      case (Action::Name::Escape):
        m_engine->changeScene("MainMenu", std::make_shared<Menu>(m_engine));
        break;
      default: break;
      }
    }

    if (action.ending()) {

    }
  }

  void PlayLevel::sRender() {
    sf::RenderWindow &window = m_engine->window();
    window.clear();

    // TODO

    window.display();
  }

}
