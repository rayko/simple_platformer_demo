/*
  EntityManager.hpp
  Main class to provide entity management, by spawning/destroying
  entities for the main game, as well as exposing them for other
  systems.
*/

#pragma once
#include "Core.hpp"
#include "Entity.hpp"
#include <map>

class EntityManager : public Core {
  EntityVector m_entities;
  EntityVector m_pendingAdditions;
  std::map<std::string, EntityVector> m_entityMap;
  size_t m_totalEntities = 0;

public:
  EntityManager() {};
  std::shared_ptr<Entity> addEntity(const std::string &tag);
  void update();
  EntityVector &entities();
  EntityVector &entities(const std::string &tag);
  const std::string str() const;
};
