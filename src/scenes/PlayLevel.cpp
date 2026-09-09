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

    sf::RenderWindow &window = m_engine->window();
    sf::Vector2f winSize = {(float) window.getSize().x, (float) window.getSize().y};
    sf::View view = sf::View(sf::FloatRect({0.0f, 0.0f}, winSize));
    window.setView(view);


    // Set the reference to world origin point. Since we are going to be
    // using a custom grid where the bottom-left corner is (0,0), we set
    // our world origin to x=0, and y=window.y which is the total y size.
    m_worldOrigin.x = 0;
    m_worldOrigin.y = m_engine->window().getSize().y;


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

  void PlayLevel::sRender() {
    sf::RenderWindow &window = m_engine->window();
    window.clear();

    sf::View view = window.getView();
    view.move({3.0f, 0.0f}); // Remove this, was just for test
    m_engine->window().setView(view);

    if (m_drawGrid)
      drawGrid();

    window.display();
  }

  void PlayLevel::drawGrid() {
    sf::RenderWindow &window = m_engine->window();
    sf::Vector2u winSize = window.getSize();
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    Vec2f viewOrigin;

    viewOrigin.x = viewCenter.x - ((float)winSize.x / 2);
    viewOrigin.y = viewCenter.y + ((float)winSize.y / 2);
    window.clear();

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
    rect.setFillColor(sf::Color(0, 0, 0, 0));
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

}
