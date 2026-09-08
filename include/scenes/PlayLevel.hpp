/*
  PlayLevel.hpp
  Main class to load a level and play it. This one should bundle all (or most)
  of the actual gameplay logic.
*/

#pragma once
#include "scenes/Base.hpp"
#include <SFML/Graphics/Font.hpp>

namespace Scenes {
  class PlayLevel : public Base {
  private:
    struct PlayerAttrs {
      float x, y, cx, cy, speed, maxSpeed, jumpVel, gravity;
      std::string weaponName;
    };

    std::shared_ptr<Entity> m_player;
    const std::string m_levelPath;
    PlayerAttrs m_playerAttrs;
    bool m_drawTextures = true;
    bool m_drawColliders = false;
    bool m_drawGrid = false;
    const Vec2f m_gridSize = {64, 64};

    std::shared_ptr<sf::Font> m_gridTextFont;
    int m_gridFontSize = 20;

    void onEnd() override;
    void init() override;
    void sDoAction(const Action &action) override;
    void sRender() override;

  public:
    PlayLevel(GameEngine *engine, std::string &lvlConfigFile);
    void update() override;
    void doAction(const Action &action) override;
  };
}
