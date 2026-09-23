#pragma once
#include "ui/PanelBase.hpp"
#include <memory>
#include <SFML/Graphics.hpp>

namespace UI {
  // Basic text panel that renders a piece of text in a box
  class TextPanel : public PanelBase {
    std::string m_text = "";
    std::shared_ptr<sf::Font> m_font;
    std::shared_ptr<sf::Text> m_textObj;
    int m_fontSize = 15;
    float m_lineSpacing = 1.8f;
    int m_textOffset = 10;

    public:
      TextPanel(std::shared_ptr<sf::Font> font);
      
      void init();
      void setText(const std::string &txt) { m_text = txt; }
      void clear() { m_text = ""; }
      void addTextLine(const std::string &txt) { m_text += std::format("{}\n", txt); };
      sf::Vector2f getSize();
      void draw(sf::RenderWindow &window);

  };
} 