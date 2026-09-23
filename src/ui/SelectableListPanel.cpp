#include "ui/SelectableListPanel.hpp"
#include <cmath>
#include <iostream>

namespace UI {

  SelectableListPanel::SelectableListPanel(std::shared_ptr<sf::Font> font) : m_font(font) {
    m_title = "List";
    init();
  }

  void SelectableListPanel::resize() {
    sf::Vector2f newSize = { 0, 0 };

    // Global paddings for the panel
    newSize.x += m_textOffset;
    newSize.y += m_textOffset;

    newSize.x = std::max(newSize.x, m_titleObj->getLocalBounds().size.x + (m_textOffset * 2));
    newSize.y += m_titleObj->getLocalBounds().size.y;

    newSize.y += m_titleBotMargin;

    for (auto textObj : m_entries) {
      newSize.x = std::max(newSize.x, textObj.getLocalBounds().size.x + (m_textOffset * 2));
      newSize.y += textObj.getLocalBounds().size.y;
      newSize.y += m_entryBotMargin;
    }

    // Do not over-space the bottom of list
    newSize.y += (m_textOffset - m_entryBotMargin);

    m_size = newSize;
  }

  void SelectableListPanel::init() {
    m_titleObj = std::make_shared<sf::Text>(*m_font, m_title, m_fontSize);
    m_titleObj->setLineSpacing(m_lineSpacing);
    m_box.setOutlineColor(m_lineColor);
    m_box.setFillColor(m_fillColor);
    m_box.setOutlineThickness(m_lineSize); 
    resize();
  }

  void SelectableListPanel::setTitle(const std::string &txt) {
    std::cout << "SETTING PANEL TITLE TO : " << txt << std::endl;
    m_title = txt;
    m_titleObj->setString(m_title);
    resize();
  }

  void SelectableListPanel::addEntry(const std::string &txt) {
    sf::Text obj(*m_font, txt, m_fontSize);
    obj.setLineSpacing(m_lineSpacing);
    obj.setFillColor(m_unSelectedColor);
    m_entries.push_back(obj);
    resize();
  }

  // Compare the given "pos" point to our obj bounds to see if the
  // click was inside any of them. If it was, we mark that item as
  // selected and update internal state.
  void SelectableListPanel::clickAt(sf::Vector2f &pos) {
    // If click is not inside our panel, do nothing
    if (!m_box.getGlobalBounds().contains(pos)) { return; }
    for (sf::Text &entry : m_entries) {
      if (entry.getGlobalBounds().contains(pos)) {
        entry.setFillColor(m_selectedColor);
        m_selectedEntryTxt = entry.getString();
        m_hasSelection = true;
        std::cout << "SELECTING " << m_selectedEntryTxt << std::endl << std::endl;
      } else {
        entry.setFillColor(m_unSelectedColor);
      }
    }
  }

  void SelectableListPanel::draw(sf::RenderWindow &window) {
    m_box.setSize(m_size);
    sf::Vector2f localPos = m_pos;
    m_box.setPosition(localPos);
    window.draw(m_box);
    localPos.x += m_textOffset;
    localPos.y += m_textOffset;
    m_titleObj->setPosition(localPos);
    window.draw(*m_titleObj);
    localPos.y += m_titleObj->getLocalBounds().size.y + m_titleBotMargin;
    for (sf::Text &entry : m_entries) {
      entry.setPosition(localPos);
      window.draw(entry);
      localPos.y += entry.getLocalBounds().size.y + m_entryBotMargin;
    }
  }

  bool SelectableListPanel::hovering(sf::Vector2f &cursor) const {
    return m_box.getGlobalBounds().contains(cursor);
  }
}