/*
  Scene.hpp
  Major structure of a "section" of game. Levels, menus, cutscenes
  and stuff like that is defined through scenes that the GameEngine
  would interconnect and switch between.

  Scenes can have a lot of internal stuff defined, they act like mini
  engines with their own entity mangaer and can do very different
  things. A level scene would play radically different from a menu scene.
*/

#pragma once
#include "Core.hpp"
#include "EntityManager.hpp"
#include "Action.hpp"
#include <map>

// This is here to fix circular dependency. GameEngine will
// require us, and we require GameEngine. Instead of using
// include and cause the problem, we make a placeholder for
// this name (and pray to compiler almigthy it works xD)
class GameEngine;

// Helper type to define map of actions for scenes.
typedef std::map<int, Action::Name> ActionMap;

namespace Scenes {
  class Base : public Core {
  protected:
    GameEngine *m_engine = nullptr;
    EntityManager m_entityManager;
    ActionMap m_actionMap;
    bool m_paused = false;
    bool m_finished = false;
    size_t m_currentFrame = 0;
    size_t m_width;
    size_t m_height;

    void setPaused(bool value);
    // Callback we can define on a scene child to run
    // when we detect it finished. Children should override
    // this.
    virtual void onEnd() = 0;

  public:
    Base() {};
    virtual ~Base() {};
    Base(GameEngine *engine) : m_engine(engine) {};

    // More functions for children to define
    virtual void update() = 0;
    virtual void init() = 0;

    virtual void sDoAction(const Action &action) = 0;
    virtual void doAction(const Action &action) = 0;

    virtual void sRender() = 0;
    void simulate(const size_t frames);
    void registerAction(int inKey, Action::Name name);
    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;
    bool isFinished() const;
    const ActionMap &getActionMap() const;
    void drawLine(const Vec2f &p1, const Vec2f &p2);
  };
}
