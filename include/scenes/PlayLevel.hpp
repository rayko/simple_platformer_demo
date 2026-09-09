/*
  PlayLevel.hpp
  Main class to load a level and play it. This one should bundle all (or most)
  of the actual gameplay logic.
*/

#pragma once
#include "scenes/Base.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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
    int m_gridFontSize = 12;
    Vec2f m_worldOrigin; // Reference to where grid (0,0) should be

    void onEnd() override;
    void init() override;
    void sDoAction(const Action &action) override;
    void sRender() override;

    // Parse gridCords custom system onto actual positions on the screen
    // to get the origin point og a single grid block.
    // Our gridCords are mosty integers like (2,3) and similar. We get
    // the world position of the origin of a block with this.
    Vec2f gridBlockOrigin(const Vec2f &gridCords) const;

    // This function takes an (x,y) coordinate in normal world space (pixel
    // coords), and returns the corresponding grid block that containes that pixel.
    Vec2f gridBlockFromPixel(const Vec2f &pos) const;
    void drawGridBlock(const Vec2f &gridCord);
    void drawGrid();

  public:
    PlayLevel(GameEngine *engine, const std::string &lvlConfigFile);
    void update() override;
    void doAction(const Action &action) override;
  };
}
