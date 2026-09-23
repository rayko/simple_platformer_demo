#include "scenes/Base.hpp"

namespace Scenes {
  void Base::setPaused(bool value) { m_paused = value; }

  void Base::simulate(const size_t frames) {
    // TODO ???
  }

  void Base::registerKeyboardAction(sf::Keyboard::Scancode inKey, Action::Name name) {
    m_keyMap[inKey] = name;
  }

  void Base::registerMouseAction(sf::Mouse::Button inBtn, Action::Name name) {
    m_mouseMap[inBtn] = name;
  }

  size_t Base::width() const { return m_width; }
  size_t Base::height() const { return m_height; }
  size_t Base::currentFrame() const { return m_currentFrame; }
  bool Base::isFinished() const { return m_finished; }
  const KeyboardMap &Base::keyMap() const { return m_keyMap; }
  const MouseMap &Base::mouseMap() const { return m_mouseMap; }

  void Base::drawLine(const Vec2f &p1, const Vec2f &p2) {
    // TODO
    // m_engine->window.draw(something)
  }

  bool Base::respondsToKey(sf::Keyboard::Scancode key) const {
    return !(m_keyMap.find(key) == m_keyMap.end());
  }

  bool Base::respondsToMouseBtn(sf::Mouse::Button btn) const {
    return !(m_mouseMap.find(btn) == m_mouseMap.end());
  }
}
