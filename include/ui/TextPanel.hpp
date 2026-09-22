#include "Vec2f.hpp"
#include <memory>
#include <SFML/Graphics.hpp>


namespace UI {
  // Basic text panel that renders a piece of text in a box
  class TextPanel {
    std::string m_text = "";
    sf::Color m_lineColor = {128, 128, 128};
    sf::Color m_fillColor = {32, 32, 32, 192};
    int m_lineSize = 1;
    sf::Vector2f m_pos = {0, 0};
    std::shared_ptr<sf::Font> m_font;
    std::shared_ptr<sf::Text> m_textObj;
    int m_fontSize = 15;
    float m_lineSpacing = 1.8f;
    int m_textOffset = 10;
    sf::RectangleShape m_box;

    public:
      TextPanel(std::shared_ptr<sf::Font> font);
      
      void init();
      void setText(const std::string &txt) { m_text = txt; }
      void clear() { m_text = ""; }
      void addTextLine(const std::string &txt) { m_text += std::format("{}\n", txt); };
      void setLineColor(sf::Color &color) { m_lineColor = color; }
      void setFillColor(sf::Color &color) { m_fillColor = color; }
      void setLineSize(int size) { m_lineSize = size; }
      void setPosition(sf::Vector2f &pos) { m_pos = pos; }

      void draw(sf::RenderWindow &window);

  };
} 