#include "scenes/Base.hpp"

namespace Scenes {
  void Base::setPaused(bool value) { m_paused = value; }

  void Base::simulate(const size_t frames) {
    // TODO ???
  }

  void Base::registerAction(int inKey, Action::Name name) {
    m_actionMap[inKey] = name;
  }

  size_t Base::width() const { return m_width; }
  size_t Base::height() const { return m_height; }
  size_t Base::currentFrame() const { return m_currentFrame; }
  bool Base::isFinished() const { return m_finished; }
  const ActionMap &Base::getActionMap() const { return m_actionMap; }

  void Base::drawLine(const Vec2f &p1, const Vec2f &p2) {
    // TODO
    // m_engine->window.draw(something)
  }
}
