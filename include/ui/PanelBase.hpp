#include <memory>
#include <SFML/Graphics.hpp>

namespace UI {
  // Basic box for panels
  class PanelBase {
    protected:
        sf::RectangleShape m_box;
        sf::Vector2f m_pos = {0, 0};
        sf::Color m_lineColor = {128, 128, 128};
        sf::Color m_fillColor = {32, 32, 32, 192};
        int m_lineSize = 1;

    public:
      PanelBase() {};
      virtual ~PanelBase() {};
      
      virtual void init() = 0;
      void setLineColor(sf::Color &color) { m_lineColor = color; }
      void setFillColor(sf::Color &color) { m_fillColor = color; }
      void setLineSize(int size) { m_lineSize = size; }
      void setPosition(sf::Vector2f &pos) { m_pos = pos; }

      virtual void draw(sf::RenderWindow &window) = 0;

  };
} 