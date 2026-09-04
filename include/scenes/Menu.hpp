/*
  Menu.hpp
  Main menu scene to show options and stuff. Should be
  the very first scene to run when booting the game.
*/

#pragma once
#include "scenes/Base.hpp"

namespace Scenes {

  class Menu : public Base {
  protected:
    std::string m_title;
    // Define menu stuff: items, text, positions, etc

    void onEnd() override;

  public:
    Menu(GameEngine *engine);
    void update() override;
    void init() override;
    void sDoAction(const Action &action) override;
    void doAction(const Action &action) override;
    void sRender() override;
  };

}
