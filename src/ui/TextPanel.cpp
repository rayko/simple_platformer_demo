#include "ui/TextPanel.hpp"

namespace UI {

  TextPanel::TextPanel(std::shared_ptr<sf::Font> font) : m_font(font) {
    init();
  }

  void TextPanel::init() {
    m_textObj = std::make_shared<sf::Text>(*m_font, m_text, m_fontSize);
    m_textObj->setLineSpacing(m_lineSpacing);
    m_textObj->setCharacterSize(m_fontSize);
    m_box.setOutlineColor(m_lineColor);
    m_box.setFillColor(m_fillColor);
    m_box.setOutlineThickness(m_lineSize);
  }

  sf::Vector2f TextPanel::getSize() {
    sf::Vector2f size(0,0);
    size.x = m_textObj->getLocalBounds().size.x + (m_textOffset * 2);
    size.y = m_textObj->getLocalBounds().size.y + (m_textOffset * 2);
    return size;
  }

  void TextPanel::draw(sf::RenderWindow &window) {
    m_box.setPosition(m_pos);
    m_textObj->setPosition(m_pos + sf::Vector2f(m_textOffset, m_textOffset));
    m_textObj->setString(m_text);
    m_box.setSize(getSize());
    window.draw(m_box);
    window.draw(*m_textObj);
  }

}