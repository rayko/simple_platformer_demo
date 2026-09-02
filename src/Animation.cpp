#include "Animation.hpp"

// Animation::Animation(const std::string &name, const sf::Texture &tex, const Vec2f size)
//   : m_animationName(name), m_sprite(tex), m_size(size) {
//   m_sprite.setTextureRect(sf::IntRect({0, 0}, size.toVector2i()));
// }

Animation::Animation(const std::string &name, const sf::Texture &tex,
                     const Vec2f size, size_t frames, size_t frameDuration)
  : m_name(name), m_size(size), m_frames(frames),
    m_frameDuration(frameDuration) {
  m_sprite = std::make_shared<sf::Sprite>(tex);
  m_sprite->setTextureRect(sf::IntRect({0, 0}, size.toVector2i()));
}

void Animation::advanceFrame() {
  if (m_frames == 0) { return; }
  m_currentFrame++;
  if (m_currentFrame > m_frames)
    m_currentFrame = 0;
}

void Animation::update() {
  advanceFrame();
  // TODO
  // TODO handle timming, where do I get frame count from?
}

// Not sure if this is right yet
bool Animation::finished() const { return m_currentFrame > m_frames; }

// TODO back and forth loop?
// TODO one shot?

const std::string &Animation::getName() const { return m_name; }
const Vec2f &Animation::getSize() const { return m_size; }
sf::Sprite &Animation::getSprite() { return *m_sprite; }

const std::string Animation::str() const {
  std::string obj = "<Animation>";
  obj += " " + m_name + " ";
  obj += (m_sprite ? " OK " : " NO ");
  obj += " F(" + std::to_string(m_frames) + ") ";
  obj += " C(" + std::to_string(m_currentFrame) + ") ";
  obj += " x " + std::to_string(m_frameDuration) + " ";
  obj += " " + m_size.str() + " ";

  return obj;
}
