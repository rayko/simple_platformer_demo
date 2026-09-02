/*
  Animation.hpp
  Defines the main Animation class that will handle
  tiles and animtations. Even though there should be
  a different class for static things like ground tiles,
  for the purpose of simplification, everything is an
  animated sprite. Static elements will get 0 frames
  when defined, so they don't animate.
*/

#pragma once
#include "Vec2f.hpp"
#include <SFML/Graphics.hpp>

class Animation {
  std::shared_ptr<sf::Sprite> m_sprite;
  size_t m_frames = 0;
  size_t m_currentFrame = 0;
  size_t m_frameDuration = 0;
  Vec2f m_size;
  std::string m_name;

  void advanceFrame();

public:
  Animation() {};
  // Animation(const std::string &name, const sf::Texture &tex, const Vec2f size);
  Animation(const std::string &name, const sf::Texture &tex, const Vec2f size,
            size_t frames, size_t frameDuration);

  void update();
  bool finished() const;
  const std::string &getName() const;
  const Vec2f &getSize() const;
  sf::Sprite &getSprite();
  const std::string str() const;
};
