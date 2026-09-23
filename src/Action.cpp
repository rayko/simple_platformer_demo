#include "Action.hpp"
#include <format>

bool Action::starting() const { return m_state == State::Start; }
bool Action::ending() const { return m_state == State::End; }
const Action::Name Action::name() const { return m_name; }
const Action::State Action::state() const { return m_state; }

const std::string Action::strState() const {
  if (m_state == State::Start) { return "START"; }
  if (m_state == State::End)   { return "END"; }
  return "UNDEFINED";
}

const std::string Action::strName() const {
  if (m_name == Name::None)            { return "NONE"; }
  if (m_name == Name::Up)              { return "UP"; }
  if (m_name == Name::Down)            { return "DOWN"; }
  if (m_name == Name::Left)            { return "LEFT"; }
  if (m_name == Name::Right)           { return "RIGHT"; }
  if (m_name == Name::Jump)            { return "JUMP"; }
  if (m_name == Name::Shoot)           { return "SHOOT"; }
  if (m_name == Name::Crouch)          { return "CROUCH"; }
  if (m_name == Name::Activate)        { return "ACTIVATE"; }
  if (m_name == Name::Escape)          { return "ESCAPE"; }
  if (m_name == Name::ToggleGrid)      { return "TOGGLE_GRID"; }
  if (m_name == Name::ToggleColliders) { return "TOGGLE_COLLIDERS"; }
  if (m_name == Name::ToggleTextures)  { return "TOGGLE_TEXTURES"; }
  if (m_name == Name::Pause)           { return "PAUSE"; }
  if (m_name == Name::ToggleInfo)      { return "TOGGLE_INFO"; }

  if (m_name == Name::LeftClick)       { return "LEFT_CLK"; }
  if (m_name == Name::RightClick)      { return "RIGHT_CLK"; }
  if (m_name == Name::MiddleClick)     { return "MID_CLK"; }
  if (m_name == Name::ScrollUp)        { return "SCRL_UP"; }
  if (m_name == Name::ScrollDown)      { return "SCRL_DOWN"; }

  return "UNDEFINED";
}


const std::string Action::toString() const {
  return std::format("{}_{}", strName(), strState());
}

const std::string Action::str() const {
  return std::format("<Action> {}", toString());
}
