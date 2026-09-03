#include "EntityManager.hpp"
#include <algorithm>
#include <format>

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
  m_pendingAdditions.push_back(entity);
  return entity;
}

EntityVector &EntityManager::entities() { return m_entities; }
EntityVector &EntityManager::entities(const std::string &tag) {
  return m_entityMap[tag];
}

void EntityManager::update() {
  for (auto &[tag, list] : m_entityMap) {
    list.erase(std::remove_if(list.begin(), list.end(),
                              [](std::shared_ptr<Entity> const &entity) {
                                return entity->isDead();
                              }),
               list.end());

  }

  m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
                                  [](std::shared_ptr<Entity> const &entity) {
                                    return entity->isDead();
                                  }),
                   m_entities.end());

  for (auto &entity : m_pendingAdditions) {
    m_entities.push_back(entity);
    m_entityMap[entity->tag()].push_back(entity);
  };
  m_pendingAdditions.clear();
};

const std::string EntityManager::str() const {
  return std::format("<EntityManager> ({}) Pending: {} Listed: {}",
                     m_totalEntities, m_pendingAdditions.size(),
                     m_entities.size());
}
