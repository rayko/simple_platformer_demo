/*
  PlayLevel.hpp
  Main class to load a level and play it. This one should bundle all (or most)
  of the actual gameplay logic.
*/

#pragma once
#include "Physics.hpp"
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

    // This is just to keep consistency on sketchy state names
    struct PlayerStates {
      const std::string stand = "STAND";
      const std::string run = "RUN";
      const std::string airborne = "AIRBORNE";
    };
    PlayerStates m_playerStates;

    std::shared_ptr<Entity> m_player;
    const std::string m_levelPath;
    PlayerAttrs m_playerAttrs;
    bool m_drawTextures = true;
    bool m_drawColliders = false;
    bool m_drawGrid = false;
    bool m_drawDebugPanel = false;
    bool m_paused = false;    
    const Vec2f m_gridSize = {64, 64};

    std::shared_ptr<sf::Font> m_gridTextFont;
    int m_gridFontSize = 12;
    Vec2f m_worldOrigin; // Reference to where grid (0,0) should be

    sf::Color m_bgColor = {128, 128, 128, 255};
    sf::Color m_bgPauseColor = {64, 64, 64, 128};

    Physics m_physics;

    bool m_playerJumping = false;
    bool m_playerOnFloor = false;

    void onEnd() override;
    void init() override;
    void sDoAction(const Action &action) override;
    void sRender() override;
    void sAnimation();
    void sMovement();
    void sCollisions();
    void sLifespan();

    // Reads level config file to set it up
    void loadLevel(const std::string &filename);

    // Parse gridCords custom system onto actual positions on the screen
    // to get the origin point og a single grid block.
    // Our gridCords are mosty integers like (2,3) and similar. We get
    // the world position of the origin of a block with this.
    Vec2f gridBlockOrigin(const Vec2f &gridCords) const;

    // This function takes an (x,y) coordinate in normal world space (pixel
    // coords), and returns the corresponding grid block that containes that pixel.
    Vec2f gridBlockFromPixel(const Vec2f &pos) const;
    void drawGrid();
    void drawColliders();
    void drawDebugPanel();
    Vec2f initialSpritePosition(const Vec2f &gridPos, const Vec2f &spriteSize);

    void spawnPlayer(const Vec2f &gridBlock);
    void spawnBullet(std::shared_ptr<Entity> player);    
    void playerHitsTile(std::shared_ptr<Entity> &tile);
    void checkBulletCollisions();

    void expireBullet(std::shared_ptr<Entity> &bullet);
    void destroyBullet(std::shared_ptr<Entity> &bullet, int direction);
    void destroyTile(std::shared_ptr<Entity> &tile);
    
  public:
    PlayLevel(GameEngine *engine, const std::string &lvlConfigFile);
    void update() override;
    void doAction(const Action &action) override;
  };
}
