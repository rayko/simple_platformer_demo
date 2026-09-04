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

  Action() {};
  Action(const std::string &n, State s) : m_name(n), m_state(s) {};

  bool starting() const;
  bool ending() const;
  const std::string &name() const;
  const State &state() const;
  const std::string toString() const;
  const std::string str() const;

private:
  const std::string m_name = "NONE";
  const State m_state = State::Start;
};
