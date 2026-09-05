#include "scenes/Menu.hpp"

namespace Scenes {
  Menu::Menu(GameEngine *engine) { m_engine = engine; }

  void Menu::onEnd() {
    // TODO
  }

  void Menu::update() {
    // TODO
  }

  void Menu::init() {
    // Map keyboard keys to actions
    registerKeyboardAction(sf::Keyboard::Scancode::W, Action::Name::Up);
    registerKeyboardAction(sf::Keyboard::Scancode::S, Action::Name::Down);
    registerKeyboardAction(sf::Keyboard::Scancode::Enter, Action::Name::Activate);
    registerKeyboardAction(sf::Keyboard::Scancode::Space, Action::Name::Activate);
    registerKeyboardAction(sf::Keyboard::Scancode::Escape, Action::Name::Escape);
  }

  void Menu::sDoAction(const Action &action) {
    // TODO
  }

  void Menu::doAction(const Action &action) {
    // TODO
  }

  void Menu::sRender() {
    // TODO
  }
}
