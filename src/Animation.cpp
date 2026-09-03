#include "Animation.hpp"

// Public

// Animation::Animation(const std::string &name, const sf::Texture &tex, const Vec2f size)
//   : m_animationName(name), m_sprite(tex), m_size(size) {
//   m_sprite.setTextureRect(sf::IntRect({0, 0}, size.toVector2i()));
// }

Animation::Animation(const std::string &name, const sf::Texture &tex, const Vec2f size)
    : m_name(name), m_size(size) {
  m_sprite = std::make_shared<sf::Sprite>(tex);
  m_sprite->setTextureRect(sf::IntRect({0, 0}, size.toVector2i()));
}

void Animation::setPlayMode(PlayMode mode) { m_playMode = mode; }
void Animation::setLoopMode(LoopMode mode) { m_loopMode = mode; }
void Animation::setFrameCount(int value) { m_frames = value; }
void Animation::setFrameDuration(int value) { m_frameDuration = value; }

void Animation::resetAnimation() {
  m_forward = true;
  m_finished = false;
  m_currentFrame = 0;
}

void Animation::update() {
  advanceFrame();
  // TODO
  // TODO handle timming, where do I get frame count from?
}

bool Animation::finished() const { return m_finished; }
const std::string &Animation::getName() const { return m_name; }
const Vec2f &Animation::getSize() const { return m_size; }
sf::Sprite &Animation::getSprite() { return *m_sprite; }

const std::string Animation::str() const {
  std::string obj = "<Animation>";
  obj += " " + m_name + " ";
  obj += (m_sprite ? "OK " : "NO ");
  obj += "F(" + std::to_string(m_frames) + ") ";
  obj += "C(" + std::to_string(m_currentFrame) + ") ";
  obj += "x " + std::to_string(m_frameDuration) + " ";
  obj += m_size.str() + " ";
  obj += "Play(" + std::to_string((int) m_playMode) + ") ";
  obj += "Loop(" + std::to_string((int) m_loopMode) + ") ";

  return obj;
}

// Private

void Animation::advanceFrame() {
  if (m_frames == 0) { return; }
  if (m_playMode == PlayMode::Static ) { return; }
  if (m_playMode == PlayMode::Once && m_finished) { return; }

  switch(m_loopMode){
  case (LoopMode::Forward):
    m_currentFrame++;
    if (m_currentFrame >= m_frames) {
      if (m_playMode == PlayMode::Once) { m_finished = true; }
      m_currentFrame = 0;
    }
    break;
  case (LoopMode::Backward):
    m_currentFrame--;
    if (m_currentFrame < 0) {
      if (m_playMode == PlayMode::Once) { m_finished = true; }
      m_currentFrame = m_frames - 1;
    }
    break;
  case (LoopMode::Bounce):
    if (m_forward) {
      if (m_currentFrame >= m_frames){
        m_forward = false;
        m_currentFrame--;
      } else {
        m_currentFrame++;
      }
    } else {
      if (m_currentFrame <= 0) {
        // When playing once in a bounce mode, when we are
        // going back to frame 0, end the animation there.
        // Effectively play once forward, and once backward.
        if (m_playMode == PlayMode::Once) { m_finished = true; }
        m_forward = true;
        m_currentFrame++;
      } else {
        m_currentFrame--;
      }
    }
    break;
  default:
    return;
    break;
  }

  // Move our area within the texture sheet to the
  // current frame to update.
  m_texturePosition.x = 0 + (m_size.x * m_currentFrame);
  m_sprite->setTextureRect(sf::IntRect(m_texturePosition.toVector2i(), m_size.toVector2i()));
}
