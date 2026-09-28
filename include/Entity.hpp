/*
  Entity.hpp
  Main Entity class to manage all objects that the game
  uses. Everything the game needs to spawn, either displayed
  on screen or not is an entity.
*/

#pragma once
#include "Components.hpp"
#include <memory>
#include <string>
#include <vector>

typedef std::tuple<CTransform, CLifespan, CInput, CBoxCollider, CAnimation,
                   CGravity, CState, CTextBox, CEventTimer, CInteractible>
    ComponentTuple;

class Entity {
  friend class EntityManager;

  size_t m_id;
  bool m_active = true;
  std::string m_tag = "default";
  ComponentTuple m_components;

  Entity(const size_t &id, const std::string &tag) : m_id(id), m_tag(tag) {};

public:
  void destroy();
  size_t id() const;
  bool isActive() const;
  bool isDead() const;
  const std::string &tag() const;
  const std::string str() const;

  template <typename T>
  bool hasComponent() const {
    return getComponent<T>().has;
  }

  template <typename T, typename... TArgs>
  T &addComponent(TArgs &&... mArgs) {
    auto &component = getComponent<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.has = true;
    return component;
  }

  template <typename T> T &getComponent() { return std::get<T>(m_components); }
  template <typename T> const T &getComponent() const {
    return std::get<T>(m_components);
  }

  template <typename T> void removeComponent() { getComponent<T>() = T(); }
};

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
