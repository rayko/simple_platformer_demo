#include "scenes/Menu.hpp"
#include "GameEngine.hpp"

namespace Scenes {
  // Public

  Menu::Menu(GameEngine *engine) : Base(engine) {
    m_logOrigin = "Scenes::Menu";
    setLogger(engine->getLogger());
    init();
  }

  void Menu::update() {
    sRender();
  }

  // This an exposed function to the public, for external stuff to send us
  // actions. It may be GameEngine, or it may be something else (ie replay
  // soruce, network, etc).
  // We can handle global scene actions here if we want, to sepparate that
  // from other internal action logic (ie player actions), as a pre-filter.
  // Otherwise, we should forward the received action to sDoAction() system
  // to handle it.
  void Menu::doAction(const Action &action) { sDoAction(action); }


  // Private

  void Menu::init() {
    logDebug("Initializing");

    // Map keyboard keys to actions
    logDebug("Mapping keyboard");
    registerKeyboardAction(sf::Keyboard::Scancode::W, Action::Name::Up);
    registerKeyboardAction(sf::Keyboard::Scancode::S, Action::Name::Down);
    registerKeyboardAction(sf::Keyboard::Scancode::Enter, Action::Name::Activate);
    registerKeyboardAction(sf::Keyboard::Scancode::Space, Action::Name::Activate);
    registerKeyboardAction(sf::Keyboard::Scancode::Escape, Action::Name::Escape);

    // Menu Entries
    logDebug("Building Menu");
    std::shared_ptr<sf::Font> font = m_engine->assetStore().getFont("SimpleFont");

    m_titleGfx = std::make_shared<sf::Text>(sf::Text(*font, "Simple Platformer Demo", m_titleCharSize));
    m_titleGfx->setFillColor(m_titleColor);

    m_menuEntries.push_back(MenuEntry(sf::Text(*font, "Entry 1", m_menuEntryCharSize), "entry1"));
    m_menuEntries.push_back(MenuEntry(sf::Text(*font, "Entry 2", m_menuEntryCharSize), "entry2"));
    m_menuEntries.push_back(MenuEntry(sf::Text(*font, "Entry 3", m_menuEntryCharSize), "entry3"));
    m_menuEntries.push_back(MenuEntry(sf::Text(*font, "Exit", m_menuEntryCharSize), "exit"));
    for (MenuEntry item : m_menuEntries) {
      item.textGfx.setFillColor(m_menuEntryColor);
    }
    m_menuIndex = 0;
  }

  // Handler of actions for the scene. This, being an ECS system should be
  // private. Most of the internal logic of actions is implemented here.
  // For menu here, we just navigate the menu with movement input.
  // We can also tell GameEngine (we should have a pointer to it here), to
  // change the scene.
  void Menu::sDoAction(const Action &action) {
    if (action.starting()) {
      switch(action.name()) {
      case (Action::Name::Escape):
        onEnd();
        break;
      case (Action::Name::Down):
        m_menuIndex++;
        if (m_menuIndex >= m_menuEntries.size())
          m_menuIndex = 0;
        break;
      case (Action::Name::Up):
        m_menuIndex--;
        if (m_menuIndex < 0)
          m_menuIndex = (m_menuEntries.size() - 1);
        break;
      case (Action::Name::Activate):
        runMenuEntry();
        break;
      default: break;
      }

      if (action.ending()) {
        // We don't need to do anything on input release
        return;
      }
    }
  }


  void Menu::onEnd() {
    // End scene routine, anything else we need to do (cleanup, save state, etc)
    // should go here, before we mark the scene done.
    // We can switch to another scene here (ie next level) too
    m_finished = true;
  }

  void Menu::sRender() {
    sf::RenderWindow &window = m_engine->window();
    window.clear();

    sf::Vector2u winSize = window.getSize();
    Vec2f titlePos{0, 0};
    // Centered
    titlePos.x = ((float)winSize.x / 2) - (m_titleGfx->getLocalBounds().size.x / 2);
    titlePos.y = m_titlePosition.y;
    m_titleGfx->setPosition(titlePos.toVector2f());

    Vec2f menuPos = m_listPosition;

    window.draw(*m_titleGfx);
    for (int idx = 0; idx < m_menuEntries.size(); idx++) {
      if (idx == m_menuIndex){
        m_menuEntries[idx].textGfx.setFillColor(m_menuEntrySelectColor);
      } else {
        m_menuEntries[idx].textGfx.setFillColor(m_menuEntryColor);
      }
      m_menuEntries[idx].textGfx.setPosition(menuPos.toVector2f());
      window.draw(m_menuEntries[idx].textGfx);
      menuPos.y += m_entryPadding + m_menuEntries[idx].textGfx.getLocalBounds().size.y;
    }
    window.display();
  }

  void Menu::runMenuEntry() {
    // TODO this should be on sDoAction() as action logic
    const std::string menuName = m_menuEntries[m_menuIndex].name;
    if (menuName == "exit") {
      m_finished = true;
      return;
    }
    logDebug("TODO Run menu action " + menuName);
  }
}
