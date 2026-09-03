#include "Entity.hpp"
#include <format>

void Entity::destroy() { m_active = false; }
size_t Entity::id() const { return m_id; }
bool Entity::isActive() const { return m_active; }
bool Entity::isDead() const { return !m_active; }
const std::string &Entity::tag() const { return m_tag; }

const std::string Entity::str() const {
  std::string status = (m_active ? "ALIVE" : "DEAD");
  return std::format("<Entity> {} {} {}", m_id, m_tag, status);
}
