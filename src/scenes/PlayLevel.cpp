#include "scenes/PlayLevel.hpp"
#include "GameEngine.hpp"
#include "scenes/Menu.hpp"

namespace Scenes {
  ///// Public

  PlayLevel::PlayLevel(GameEngine *engine, const std::string &lvlConfigFile)
    : m_levelPath(lvlConfigFile), Base(engine) {

    m_logOrigin = "Scenes::PlayLevel (" + m_levelPath + ")";
    setLogger(engine->getLogger());
    init();
  }

  void PlayLevel::update() {
    m_entityManager.update();
    // m_view.move({3, 0});
    m_engine->window().setView(m_view);

    sAnimation();
    sRender();
  }

  void PlayLevel::doAction(const Action &action) { sDoAction(action); }


  ///// Private

  void PlayLevel::init() {
    // TODO
    logInfo("Loading scene");
    m_gridTextFont = m_engine->assetStore().getFont("SimpleFont");

    logDebug("Mapping actions");
    registerKeyboardAction(sf::Keyboard::Scancode::G, Action::Name::ToggleGrid);
    registerKeyboardAction(sf::Keyboard::Scancode::T, Action::Name::ToggleTextures);
    registerKeyboardAction(sf::Keyboard::Scancode::C, Action::Name::ToggleColliders);
    registerKeyboardAction(sf::Keyboard::Scancode::Escape, Action::Name::Escape);
    registerKeyboardAction(sf::Keyboard::Scancode::P, Action::Name::Escape);

    m_width = m_engine->window().getSize().x;
    m_height = m_engine->window().getSize().y;
    m_view = sf::View(sf::FloatRect({0, 0}, {(float)m_width, (float)m_height}));
    m_engine->window().setView(m_view);

    // Set the reference to world origin point. Since we are going to be
    // using a custom grid where the bottom-left corner is (0,0), we set
    // our world origin to x=0, and y=window.y which is the total y size.
    m_worldOrigin = {0, (float)m_height};

    m_entityManager = EntityManager();
    spawnTestEntities();
  }

  void PlayLevel::onEnd() {
    // TODO
    m_finished = true;
  }

  void PlayLevel::sDoAction(const Action &action) {
    if (action.starting()) {
      switch(action.name()) {
      case (Action::Name::ToggleGrid):
        m_drawGrid = !m_drawGrid;
        break;
      case (Action::Name::ToggleTextures):
        m_drawTextures = !m_drawTextures;
        break;
      case (Action::Name::ToggleColliders):
        m_drawColliders = !m_drawColliders;
        break;
      case (Action::Name::Escape):
        m_engine->changeScene("MainMenu", std::make_shared<Menu>(m_engine));
        break;
      default: break;
      }
    }

    if (action.ending()) {

    }
  }

  void PlayLevel::sAnimation() {
    for (auto entity : m_entityManager.entities()) {
      if (entity->hasComponent<CAnimation>()) {
        auto anim = m_engine->assetStore().getAnimation(entity->getComponent<CAnimation>().name);
        if (anim->finished()) {
          entity->destroy();
        } else {
          anim->getSprite().setPosition(entity->getComponent<CTransform>().pos.toVector2f());
          if (anim->getName() == "AlexRun")
            logDebug("Updating animation " + anim->str());
          anim->update();
        }
      }

    }
  }

  void PlayLevel::sMovement() {


  }

  void PlayLevel::sRender() {
    sf::RenderWindow &window = m_engine->window();
    window.clear();

    if (m_drawTextures) {
      for (auto entity : m_entityManager.entities()){
        if (entity->hasComponent<CAnimation>()) {
          auto anim = m_engine->assetStore().getAnimation(entity->getComponent<CAnimation>().name);
          window.draw(anim->getSprite());
        }
      }
    }

    if (m_drawGrid)
      drawGrid();

    window.display();
  }

  void PlayLevel::drawGrid() {
    sf::RenderWindow &window = m_engine->window();
    sf::Vector2f viewCenter = m_view.getCenter();
    Vec2f viewOrigin;

    viewOrigin.x = viewCenter.x - ((float) m_width / 2);
    viewOrigin.y = viewCenter.y + ((float) m_height / 2);

    Vec2f gridBlock = gridBlockFromPixel(viewOrigin);
    // TODO dynamically place grid based on view size instead of hardcoding
    for (int y = 0; y <= 12; y++)
      for (int x = 0; x <= 20; x++)
        drawGridBlock(Vec2f{gridBlock.x + x, gridBlock.y + y});
  }

  // Draws a single grid block of the given grid coordinates in world sapce.
  void PlayLevel::drawGridBlock(const Vec2f &gridCord) {
    Vec2f pos = gridBlockOrigin(gridCord);
    sf::RectangleShape rect;
    rect.setSize(m_gridSize.toVector2f());
    rect.setOutlineColor(sf::Color(255, 255, 255, 128));
    rect.setOutlineThickness(-1);
    rect.setFillColor(sf::Color::Transparent);
    rect.setPosition(pos.toVector2f());

    const std::string txt = std::format("{}, {}", (int) gridCord.x, (int) gridCord.y);
    sf::Text blockName(*m_gridTextFont, txt, m_gridFontSize);
    blockName.setPosition(sf::Vector2f(pos.x + 10, pos.y - 20));

    m_engine->window().draw(rect);
    m_engine->window().draw(blockName);
  }

  Vec2f PlayLevel::gridBlockOrigin(const Vec2f &gridCords) const {
    float y = m_worldOrigin.y;
    Vec2f origin = {0, y};
    origin.x += (m_gridSize.x * gridCords.x);
    origin.y -= (m_gridSize.y * gridCords.y);
    return origin;
  }

  Vec2f PlayLevel::gridBlockFromPixel(const Vec2f &pos) const {
    int x = std::floor(pos.x);
    x -= (x % (int)std::floor(m_gridSize.x));

    int y = std::floor(pos.y) - m_engine->window().getSize().y;
    y -= (y % (int)std::floor(m_gridSize.y));
    y += m_gridSize.y; // Fix value to target bottom corner instead of top corner

    return Vec2f(x / m_gridSize.x, (y / m_gridSize.y) - 1);
  }

  void PlayLevel::spawnTestEntities() {
    std::shared_ptr<Entity> entity;
    Vec2f gridBlock = {0, 0};
    Vec2f pos;

    entity = m_entityManager.addEntity("tile");
    gridBlock = {5, 5};
    pos = gridBlockOrigin(gridBlock);
    entity->addComponent<CTransform>(pos);
    entity->addComponent<CAnimation>("TileGnd1");

    entity = m_entityManager.addEntity("tile");
    gridBlock = {7, 5};
    pos = gridBlockOrigin(gridBlock);
    entity->addComponent<CTransform>(pos);
    entity->addComponent<CAnimation>("AlexRun");

    entity = m_entityManager.addEntity("tile");
    gridBlock = {9, 5};
    pos = gridBlockOrigin(gridBlock);
    entity->addComponent<CTransform>(pos);
    entity->addComponent<CAnimation>("AlexStand");

    entity = m_entityManager.addEntity("tile");
    gridBlock = {11, 5};
    pos = gridBlockOrigin(gridBlock);
    entity->addComponent<CTransform>(pos);
    entity->addComponent<CAnimation>("Cloud1");

    entity = m_entityManager.addEntity("tile");
    gridBlock = {5, 7};
    pos = gridBlockOrigin(gridBlock);
    entity->addComponent<CTransform>(pos);
    entity->addComponent<CAnimation>("ActiveTile1");

    entity = m_entityManager.addEntity("tile");
    gridBlock = {7, 7};
    pos = gridBlockOrigin(gridBlock);
    entity->addComponent<CTransform>(pos);
    entity->addComponent<CAnimation>("Explosion");
  }

}
