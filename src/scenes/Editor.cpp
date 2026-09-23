#include "scenes/Editor.hpp"
#include "GameEngine.hpp"
#include "scenes/Menu.hpp"
#include <fstream>
#include <cmath>
#include <filesystem>

namespace Scenes {
  ///// Public

  Editor::Editor(GameEngine *engine) : m_levelPath("editor_level.txt"), Base(engine) {
    m_logOrigin = "Scenes::Editor (" + m_levelPath + ")";
    m_receivePointerLocation = true;
    setLogger(engine->getLogger());
    init();
  }

  void Editor::update() {
    m_entityManager.update();

    sMovement();
    sRender();
  }

  void Editor::doAction(const Action &action) { sDoAction(action); }


  ///// Private

  void Editor::init() {
    logInfo("Loading scene");
    m_gridTextFont = m_engine->assetStore().getFont("SimpleFont");

    logDebug("Mapping actions");
    registerKeyboardAction(sf::Keyboard::Scancode::F1, Action::Name::ToggleGrid);
    registerKeyboardAction(sf::Keyboard::Scancode::F2, Action::Name::ToggleFrontDec);
    registerKeyboardAction(sf::Keyboard::Scancode::F3, Action::Name::ToggleTiles);
    registerKeyboardAction(sf::Keyboard::Scancode::F4, Action::Name::ToggleBackDec);
    registerKeyboardAction(sf::Keyboard::Scancode::F5, Action::Name::SaveLevel);
    registerKeyboardAction(sf::Keyboard::Scancode::Escape, Action::Name::Escape);

    registerKeyboardAction(sf::Keyboard::Scancode::W, Action::Name::Up);
    registerKeyboardAction(sf::Keyboard::Scancode::S, Action::Name::Down);
    registerKeyboardAction(sf::Keyboard::Scancode::A, Action::Name::Left);
    registerKeyboardAction(sf::Keyboard::Scancode::D, Action::Name::Right);

    registerMouseAction(sf::Mouse::Button::Left, Action::Name::LeftClick);
    registerMouseWheelAction(Action::MWheelEvent::ScrollUp, Action::Name::ScrollUp);
    registerMouseWheelAction(Action::MWheelEvent::ScrollDown, Action::Name::ScrollDown);

    m_width = m_engine->window().getSize().x;
    m_height = m_engine->window().getSize().y;
    m_view = sf::View(sf::FloatRect({0, 0}, {(float)m_width, (float)m_height}));
    m_engine->window().setView(m_view);

    // Set the reference to world origin point. Since we are going to be
    // using a custom grid where the bottom-left corner is (0,0), we set
    // our world origin to x=0, and y=window.y which is the total y size.
    m_worldOrigin = {0, (float)m_height};

    m_entityManager = EntityManager();

    // Spawn level file for editor with default config
    if (!std::filesystem::exists(m_levelPath)) {
      std::ofstream file(m_levelPath);
      file << "# Editor level - Rename file after done to edit a new blank level" << std::endl;

      // Default Level settings
      file << "BackgroundColor 82 64 128" << std::endl;
      file << "Player 6 5 42 64 5.5  12 20 0.8 default" << std::endl << std::endl;
      file.close();
    }

    ui_helpInfo = std::make_shared<UI::TextPanel>(m_gridTextFont);
    ui_helpInfo->addTextLine("Help");
    ui_helpInfo->addTextLine("  W, A, S, D  -> Move"); 
    ui_helpInfo->addTextLine("  F1          -> Toggle Grid"); 
    ui_helpInfo->addTextLine("  F2          -> Toggle Front Decorations"); 
    ui_helpInfo->addTextLine("  F3          -> Toggle Tiles"); 
    ui_helpInfo->addTextLine("  F4          -> Toggle Back Decorations"); 
    ui_helpInfo->addTextLine("  F5          -> Save Level"); 
    ui_helpInfo->addTextLine("  ESC         -> Quit"); 

    ui_tileList = std::make_shared<UI::SelectableListPanel>(m_gridTextFont);
    ui_tileList->setTitle("Tiles");
    ui_tileList->addEntry("SomeGrass1");
    ui_tileList->addEntry("Block2");
    ui_tileList->addEntry("SomeOtherTile");

    loadTileNames();
    loadLevel(m_levelPath);
  }

  void Editor::loadTileNames() {
    for (auto &[name, anim] : m_engine->assetStore().animations()) {
      if (anim.type == "Tile") {
        m_tileNames.push_back(name);
      }
    }
  }

  void Editor::loadLevel(const std::string &filename) {
    logInfo("Loading config " + filename);
    std::ifstream fin = openFile(filename);
    logDebug("Loading level...");
    std::string token;
    Vec2f gridPos;
    Vec2f worldPos;
    std::shared_ptr<Animation> anim;
    std::shared_ptr<Entity> entity;
    std::string animName;
    int value;
    while (fin >> token) {
      // Ignore comments
      if (token.starts_with("#")) {
        fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }

      if (token == "Player"){
        // Load player attrs
        fin >> m_playerAttrs.x >> m_playerAttrs.y;   // Grid Position (x,y)
        fin >> m_playerAttrs.cx >> m_playerAttrs.cy; // Collider size (x,y)
        fin >> m_playerAttrs.speed;                  // X speed (run/move)
        fin >> m_playerAttrs.jumpVel;                // Jump speed (Y)
        fin >> m_playerAttrs.maxSpeed;               // max speed (x or y)
        fin >> m_playerAttrs.gravity;                // Duh!
        fin >> m_playerAttrs.weaponName;             // Texture for bullet
      } else if (token == "Tile") {
        // Create all tiles
        fin >> animName;
        fin >> gridPos.x >> gridPos.y;

        entity = m_entityManager.addEntity("Tile");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation(animName));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
      } else if (token == "FrontDec") {
        // Create all decorations
        fin >> animName;
        fin >> gridPos.x >> gridPos.y;

        entity = m_entityManager.addEntity("FrontDec");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation(animName));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
      } else if (token == "BackDec") {
        // Create all decorations
        fin >> animName;
        fin >> gridPos.x >> gridPos.y;

        entity = m_entityManager.addEntity("BackDec");
        entity->addComponent<CAnimation>(m_engine->assetStore().getAnimation(animName));
        anim = entity->getComponent<CAnimation>().animation;
        worldPos = initialSpritePosition(gridPos, anim->getSize());
        entity->addComponent<CTransform>(worldPos);
      } else if (token == "BackgroundColor") {
        fin >> value;
        m_bgColor.r = value;
        fin >> value;
        m_bgColor.g = value;
        fin >> value;
        m_bgColor.b = value;
      } else {
        logWarn("Unrecognized keyword: " + token);
      }
    }
  }

  void Editor::onEnd() {
    // TODO
    m_finished = true;
  }


  void Editor::sDoAction(const Action &action) {
    sf::Vector2f cursor;
    // Reserved for future scroll events
    if (action.triggered()) {
      switch (action.name()) {
      default: break;
      }
    }

    if (action.starting()) {
      switch (action.name()) {
      case (Action::Name::LeftClick):
        cursor.x = viewCursorPosition().x;
        cursor.y = viewCursorPosition().y;
        if (ui_tileList->hovering(cursor)) {
          ui_tileList->clickAt(cursor);
        }
        break;
      case (Action::Name::ToggleGrid):
        m_drawGrid = !m_drawGrid;
        break;
      case (Action::Name::ToggleFrontDec):
        m_drawFrontDec = !m_drawFrontDec;
        break;
      case (Action::Name::ToggleTiles):
        m_drawTiles = !m_drawTiles;
        break;
      case (Action::Name::ToggleBackDec):
        m_drawBackDec = !m_drawBackDec;
        break;
      case (Action::Name::Escape):
        m_engine->changeScene("MainMenu", std::make_shared<Menu>(m_engine));
        break;
      case (Action::Name::Up):
        m_moveUp = true;
        break;
      case (Action::Name::Down):
        m_moveDown = true;
        break;
      case (Action::Name::Left):
        m_moveLeft = true;
        break;
      case (Action::Name::Right):
        m_moveRight = true;
        break;
      default: break;
      }
    }

    if (action.ending()) {
      switch (action.name()) {
      case (Action::Name::Up):
        m_moveUp = false;
        break;
      case (Action::Name::Down):
        m_moveDown = false;
        break;
      case (Action::Name::Left):
        m_moveLeft = false;
        break;
      case (Action::Name::Right):
        m_moveRight = false;
        break;
      default: break;
      }
    }
  }

  void Editor::sMovement() {
    // Move camera
    sf::Vector2f vel = {0, 0};
    if (m_moveRight)
      vel.x = m_cameraSpeed;
    if (m_moveUp)
      vel.y = -m_cameraSpeed;
    if (m_moveDown)
      vel.y = m_cameraSpeed;
    if (m_moveLeft)
      vel.x = -m_cameraSpeed;
    sf::Vector2f cameraCenter = m_view.getCenter();

    // Limit camera movement
    if (vel.x < 0 && cameraCenter.x <= m_width / 2)
      vel.x = 0;
    if (vel.y > 0 && cameraCenter.y >= m_height / 2)
      vel.y = 0;
    m_view.move(vel);
    m_viewCenter = m_view.getCenter();
  }

  void Editor::sRender() {
    sf::RenderWindow &window = m_engine->window();

    m_engine->window().setView(m_view);

    window.clear(m_bgColor);

    if (m_drawBackDec) {
      for (auto entity : m_entityManager.entities("BackDec")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }
    }

    if (m_drawTiles) {
      for (auto entity : m_entityManager.entities("Tile")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }
    }

    if (m_drawFrontDec) {
      for (auto entity : m_entityManager.entities("FrontDec")) {
        if (entity->hasComponent<CAnimation>()) {
          window.draw(entity->getComponent<CAnimation>().animation->getSprite());
        }
      }
    }

    if (m_drawGrid)
      drawGrid();

    drawCursorCords();
    drawCursor();

    sf::Vector2f pos = { 0, 0 };
    pos.x = m_viewCenter.x + (m_width / 2) - ui_helpInfo->getSize().x - 10;
    pos.y = m_viewCenter.y + (m_height / 2) - ui_helpInfo->getSize().y - 10;
    ui_helpInfo->setPosition(pos);
    ui_helpInfo->draw(window);

    pos.x = 0;
    pos.y = 10;
    pos.x = m_viewCenter.x + (m_width / 2) - ui_tileList->getSize().x - 10;
    pos.y = m_viewCenter.y - (m_height / 2) + 10;
    ui_tileList->setPosition(pos);
    ui_tileList->draw(window);

    window.display();
  }

  Vec2f Editor::viewCursorPosition() {
    return Vec2f(m_pointerPos.x + m_viewCenter.x - (m_width / 2),
                m_pointerPos.y + m_viewCenter.y - (m_height / 2));
  }

  void Editor::drawCursor() {
    sf::RectangleShape rect;
    rect.setSize(m_gridSize.toVector2f());
    rect.setOutlineColor(sf::Color(255, 0, 0));
    rect.setOutlineThickness(1);
    rect.setFillColor(sf::Color::Transparent);
    Vec2f cursor = viewCursorPosition();
    Vec2f pos = gridBlockFromPixel(cursor);
    rect.setPosition(gridBlockOrigin(pos).toVector2f());
    m_engine->window().draw(rect);
  }

  // void Editor::drawTileListPanel() {
  //   sf::RectangleShape rect;
  //   rect.setOutlineColor(sf::Color::Green);
  //   rect.setOutlineThickness(2);
  //   rect.setFillColor(sf::Color(64,64,64,128));
  //   Vec2f panelSize = { 0,0 };
  //   sf::Text name(*m_gridTextFont, "", 13);
  //   for (std::string tileName : m_tileNames) {

  //   }
  // }

  void Editor::drawCursorCords() {
    sf::Text text(*m_gridTextFont, "", 15);
    text.setString(m_pointerPos.str());
    text.setPosition(sf::Vector2f(10, 10));
    m_engine->window().draw(text);
  }

  void Editor::drawGrid() {
    sf::RenderWindow &window = m_engine->window();
    sf::Vector2f viewCenter = m_view.getCenter();
    Vec2f viewOrigin;
    Vec2f pos;
    sf::RectangleShape rect;
    rect.setSize(m_gridSize.toVector2f());
    rect.setOutlineColor(sf::Color(255, 255, 255, 128));
    rect.setOutlineThickness(-1);
    rect.setFillColor(sf::Color::Transparent);
    sf::Text blockName(*m_gridTextFont, "", m_gridFontSize);

    viewOrigin.x = viewCenter.x - ((float) m_width / 2);
    viewOrigin.y = viewCenter.y + ((float) m_height / 2);

    Vec2f gridBlock = gridBlockFromPixel(viewOrigin);
    Vec2f currentGridBlock;

    // TODO dynamically place grid based on view size instead of hardcoding
    for (int y = 0; y <= 12; y++)
      for (int x = 0; x <= 20; x++) {
        currentGridBlock.x = gridBlock.x + x;
        currentGridBlock.y = gridBlock.y + y;
        pos = gridBlockOrigin(currentGridBlock);
        rect.setPosition(pos.toVector2f());
        blockName.setString(std::format("{}, {}", (int) currentGridBlock.x, (int) currentGridBlock.y));
        blockName.setPosition(sf::Vector2f(pos.x + 5, pos.y + 50));

        m_engine->window().draw(rect);
        m_engine->window().draw(blockName);
      }
  }

  Vec2f Editor::gridBlockOrigin(const Vec2f &gridCords) const {
    float y = m_worldOrigin.y;
    Vec2f origin = {0, y};
    origin.x += (m_gridSize.x * gridCords.x);
    origin.y -= (m_gridSize.y * gridCords.y);
    origin.y -= m_gridSize.y;
    return origin;
  }

  Vec2f Editor::gridBlockFromPixel(const Vec2f &pos) const {
    int x = std::floor(pos.x);
    x -= (x % (int)std::floor(m_gridSize.x));

    int y = std::floor(pos.y) - m_engine->window().getSize().y;
    y -= (y % (int)std::floor(m_gridSize.y));

    return Vec2f(x / m_gridSize.x, -(y / m_gridSize.y));
  }

  // Derives a proper location from a grid coordinate onto
  // a world coordinate to place a sprite at the desired
  // grid position, ensuring the bottom of the sprite aligns
  // with the bottom line of the corresponding grid position.
  // Sprites bigger than 64 pixels need extra adjustment.
  // We assume all sprites set their origin at their center here.
  Vec2f Editor::initialSpritePosition(const Vec2f &gridPos, const Vec2f &spriteSize) {
    // Grid block origin references top left corner of the block
    Vec2f worldPos = gridBlockOrigin(gridPos);

    worldPos.x += spriteSize.x / 2; // Offset to the right

    // Sprites are drawn from a point towards bottom-right.
    // Sprites that are 64 pixels tall need only half-size
    // on the Y offset. Taller sprites require more logic.
    if (spriteSize.y > m_gridSize.y) {

    } else {
      worldPos.y += spriteSize.y / 2; // Offset down
    }
    return worldPos;
  }

}
