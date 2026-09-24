/*
  PlayLevel.hpp
  Main class to load a level and play it. This one should bundle all (or most)
  of the actual gameplay logic.
*/

#pragma once
#include "scenes/Base.hpp"
#include "ui/TextPanel.hpp"
#include "ui/SelectableListPanel.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Scenes {
  class Editor : public Base {
  private:
    struct PlayerAttrs {
      float x, y, cx, cy, speed, maxSpeed, jumpVel, gravity;
      std::string weaponName;
    };

    const std::string m_levelPath;
    PlayerAttrs m_playerAttrs;

    // Raw inputs to move camera
    bool m_moveUp = false;
    bool m_moveDown = false;
    bool m_moveLeft = false;
    bool m_moveRight = false;
    bool m_placeTiles = false;
    bool m_removeTiles = false;
    float m_cameraSpeed = 8.0f;

    bool m_drawFrontDec = true;
    bool m_drawTiles = true;
    bool m_drawBackDec = true;
    bool m_drawGrid = false;
    const Vec2f m_gridSize = {64, 64};

    std::vector<std::string> m_tileNames;

    std::shared_ptr<sf::Font> m_gridTextFont;
    int m_gridFontSize = 12;
    Vec2f m_worldOrigin; // Reference to where grid (0,0) should be

    sf::Color m_bgColor = {128, 128, 128, 255};
    sf::Color m_bgPauseColor = {64, 64, 64, 128};

    std::map<std::string, std::shared_ptr<Entity>> m_tiles;
    std::map<std::string, std::shared_ptr<Entity>> m_fronDecTiles;
    std::map<std::string, std::shared_ptr<Entity>> m_backDecTiles;

    // UI Stuff
    std::shared_ptr<UI::TextPanel> ui_helpInfo;
    std::shared_ptr<UI::SelectableListPanel> ui_tileList;
    std::shared_ptr<UI::SelectableListPanel> ui_placementType;

    void onEnd() override;
    void init() override;
    void sDoAction(const Action &action) override;
    void sRender() override;
    void sMovement();
    void sAnimation();
    void sTilingActions();

    void placeTile(Vec2f &gridPos);
    void removeTile(Vec2f &gridPos);
    std::shared_ptr<Entity> spawnEntity(const Vec2f &gridPos, const std::string &tag, const std::string &animName);

    // Reads level config file to set it up
    void loadLevel(const std::string &filename);
    void saveLevel(const std::string &filename);
    void writeLevelFileDefaults(std::ofstream &outputFile);
    void loadTileNames();

    // Parse gridCords custom system onto actual positions on the screen
    // to get the origin point og a single grid block.
    // Our gridCords are mosty integers like (2,3) and similar. We get
    // the world position of the origin of a block with this.
    Vec2f gridBlockOrigin(const Vec2f &gridCords) const;

    // This function takes an (x,y) coordinate in normal world space (pixel
    // coords), and returns the corresponding grid block that containes that pixel.
    Vec2f gridBlockFromPixel(const Vec2f &pos) const;
    void drawGrid();
    void drawCursor();
    void drawCursorCords();
    void drawTileListPanel();
    Vec2f viewCursorPosition();
    Vec2f initialSpritePosition(const Vec2f &gridPos, const Vec2f &spriteSize);

  public:
    Editor(GameEngine *engine);
    void update() override;
    void doAction(const Action &action) override;
  };
}
