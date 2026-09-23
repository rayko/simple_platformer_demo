#pragma once
#include "ui/PanelBase.hpp"
#include <memory>
#include <SFML/Graphics.hpp>

namespace UI {
  // Basic text panel that renders a piece of text in a box
  class SelectableListPanel : public PanelBase {
    std::string m_title = "";
    std::shared_ptr<sf::Text> m_titleObj;
    std::vector<sf::Text> m_entries;
    std::shared_ptr<sf::Font> m_font;
    std::string m_selectedEntryTxt = "";
    sf::Color m_selectedColor = { 128, 128, 255 };
    sf::Color m_unSelectedColor = { 192, 192, 192 };
    sf::Vector2f m_size = { 0, 0 };

    bool m_hasSelection = false;
    int m_fontSize = 15;
    float m_lineSpacing = 1.8f;
    int m_textOffset = 10;
    int m_titleBotMargin = 20;
    int m_entryBotMargin = 10;
    int m_firstEntryIdx = 0;
    int m_maxdisplayItems = 4;

    void resize();

    public:
      SelectableListPanel(std::shared_ptr<sf::Font> font);

      void init();
      bool hasSelection() { return m_hasSelection; }
      const std::string &selectionText() { return m_selectedEntryTxt; }
      void setMaxDisplayItems(int value) { m_maxdisplayItems = value; }
      void setTitle(const std::string &txt);
      void addEntry(const std::string &txt);
      sf::Vector2f getSize() { return m_size; };
      void clickAt(sf::Vector2f &pos);
      bool hovering(sf::Vector2f &cursor) const;
      void draw(sf::RenderWindow &window);
      void scrollUp();
      void scrollDown();
      void preSelect(const std::string &txt);
  };
} 