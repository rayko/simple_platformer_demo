#include "Scene.hpp"

void Scene::setPaused(bool value) { m_paused = value; }

void Scene::simulate(const size_t frames) {
  // TODO ???
}

void Scene::registerAction(int inKey, Action::Name name) {
  m_actionMap[inKey] = name;
}

size_t Scene::width() const { return m_width; }
size_t Scene::height() const { return m_height; }
size_t Scene::currentFrame() const { return m_currentFrame; }
bool Scene::isFinished() const { return m_finished; }
const ActionMap &Scene::getActionMap() const { return m_actionMap; }

void Scene::drawLine(const Vec2f &p1, const Vec2f &p2) {
  // TODO
}
