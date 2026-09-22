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

  void TextPanel::draw(sf::RenderWindow &window) {
    const sf::Vector2f offset(m_textOffset, m_textOffset);
    sf::Vector2f boxSize = { 0, 0 };
    m_box.setPosition(m_pos);
    m_textObj->setPosition(m_pos + offset);
    m_textObj->setString(m_text);
    boxSize.x = m_textObj->getLocalBounds().size.x + (offset.x * 2);
    boxSize.y = m_textObj->getLocalBounds().size.y + (offset.y * 2);
    m_box.setSize(boxSize);
    window.draw(m_box);
    window.draw(*m_textObj);
  }

}