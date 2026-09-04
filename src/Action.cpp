#include "Action.hpp"
#include <format>

bool Action::starting() const { return m_state == State::Start; }
bool Action::ending() const { return m_state == State::End; }
const std::string &Action::name() const { return m_name; }
const Action::State &Action::state() const { return m_state; }

const std::string Action::toString() const {
  std::string state_name;
  switch (m_state){
  case (State::Start):
    state_name = "START";
    break;
  case (State::End):
    state_name = "END";
    break;
  default:
    state_name = "UNDEFINED";
    break;
  }
  return std::format("{}_{}", m_name, state_name);
}

const std::string Action::str() const {
  return std::format("<Action> {}", toString());
}
