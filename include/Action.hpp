/*
  Action.hpp
  Small structure to carry information about actions
  a scene may execute, if the scene responds to that
  action. We abstract key presses this way and instead
  of sending keypresses to the scene, we send actions.
*/

#pragma once
#include <string>

class Action {
public:
  enum class State { Start, End };
  enum class Name {
    None,
    Up,
    Down,
    Left,
    Right,
    Jump,
    Shoot,
    Crouch,
    Activate,
    Escape,
    ToggleGrid,
    ToggleColliders,
    ToggleTextures,
    Pause,
    ToggleInfo,
    ToggleFrontDec,
    ToggleBackDec,
    ToggleTiles,
    SaveLevel,
    LeftClick,
    RightClick,
    MiddleClick,
    ScrollUp,
    ScrollDown
  };


  Action() {};
  Action(Name n, State s) : m_name(n), m_state(s) {};

  bool starting() const;
  bool ending() const;
  const Name name() const;
  const State state() const;
  const std::string strName() const;
  const std::string strState() const;
  const std::string toString() const;
  const std::string str() const;

private:
  const Name m_name = Name::None;
  const State m_state = State::Start;
};
