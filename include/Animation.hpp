/*
  Animation.hpp
  Defines the main Animation class that will handle
  tiles and animtations. Even though there should be
  a different class for static things like ground tiles,
  for the purpose of simplification, everything is an
  animated sprite. Static elements will get 0 frames
  when defined, so they don't animate.

  All animations will use a single texture. Most will
  be static, meaning a singular block in the texture to
  use, however, for actual animated sprites, the texture
  will have to place all of its frames in order on a single
  row, and only one animation per texture sheet.

  IntRect is what SFML Sprite use on the given texture to
  select the image area to display. All textures used start
  at (0,0) as position and use the given size to makeup the
  IntRect.
*/

#pragma once
#include "Vec2f.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class Animation {
public:
  // Determines if the animation plays or not, and how.
  // Static = do not animate
  // Once = play once and do not animate anymore
  // Loop = continually play the animation
  enum class PlayMode { Static, Once, Loop };

  // Determines the direction to loop, when PlayMode::Loop is set
  // Forward = increase m_currentFrame and loop back to frame 0
  // Backward = decrease m_currentFrame and loop back to m_frames
  // Bounce = increase until end, then decrease until start
  // (Bounce will always start forward)
  // (Bounc with PlayMode::Once will end after 1 bounce)
  enum class LoopMode { Forward, Backward, Bounce };

  Animation() {};
  // Animation(const std::string &name, const sf::Texture &tex, const Vec2f size);
  Animation(const std::string &name, const sf::Texture &tex, const Vec2f size);

  void update();
  bool finished() const;

  // Basically reset the animation to frame 0
  void resetAnimation();

  void setPlayMode(PlayMode mode);
  void setLoopMode(LoopMode mode);
  void setFrameCount(int value);
  void setFrameDuration(int value);

  const std::string &getName() const;
  const Vec2f &getSize() const;
  sf::Sprite &getSprite();
  const std::string str() const;

private:
  std::string m_name;
  std::shared_ptr<sf::Sprite> m_sprite;
  size_t m_frames = 0;
  size_t m_currentFrame = 0;
  size_t m_frameDuration = 0;

  // These 2 vectors will land as a IntRect on the sprite,
  // defining the section of the texture for the frame. We
  // move m_texturePosition X value to set a frame position
  // based on multiples of m_size.x.
  Vec2f m_texturePosition{0,0};
  Vec2f m_size{64,64};

  bool m_forward = true;
  bool m_finished = false;
  PlayMode m_playMode = PlayMode::Static;
  LoopMode m_loopMode = LoopMode::Forward;

  void advanceFrame();

};
