/*
  Menu.hpp
  Main menu scene to show options and stuff. Should be
  the very first scene to run when booting the game.
*/

#pragma once
#include "scenes/Base.hpp"
#include <SFML/Graphics/Text.hpp>

namespace Scenes {

  class Menu : public Base {
  private:
    struct MenuEntry{
      sf::Text textGfx;
      std::string name;
    };

  protected:
    std::shared_ptr<sf::Text> m_titleGfx;
    std::string m_title;
    int m_titleCharSize = 80;
    sf::Color m_titleColor = {255, 255, 255};

    std::vector<MenuEntry> m_menuEntries;
    sf::Color m_menuEntryColor = {255, 100, 100};
    sf::Color m_menuEntrySelectColor = {100, 255, 100};
    int m_menuIndex = 0;
    int m_menuEntryCharSize = 50;

    Vec2f m_titlePosition = {20, 20};
    Vec2f m_listPosition = {50, 150};
    int m_entryPadding = 20;
    // Define menu stuff: items, text, positions, etc

    void onEnd() override;

  public:
    using Base::Base;
    void update() override;
    void init() override;
    void sDoAction(const Action &action) override;
    void doAction(const Action &action) override;
    void sRender() override;
  };

}
