#include "scenes/Menu.hpp"
#include "GameEngine.hpp"

namespace Scenes {
  void Menu::onEnd() {
    // TODO
  }

  void Menu::update() {
    sRender();
  }

  void Menu::init() {
    // TODO Find a better way to set this, I can't set it in constructor because it messes stuff up
    m_logOrigin = "Scenes::Menu";

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

  void Menu::sDoAction(const Action &action) {
    // TODO Not sure what this should do yet
  }

  void Menu::doAction(const Action &action) {
    // Ignoring action as "ending", we care about key press, not key releases here
    if (action.ending()) return;
    switch(action.name()){
    case (Action::Name::Escape):
      // TODO should call onEnd() when exiting
      if (action.starting()) { m_finished = true; }
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
    const std::string menuName = m_menuEntries[m_menuIndex].name;
    if (menuName == "exit") {
      m_finished = true;
      return;
    }
    logDebug("TODO Run menu action " + menuName);
  }
}
