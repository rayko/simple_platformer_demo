/*
  Components.hpp
  These are the possible components to attach to
  Entities and read when processing entities. They
  are essentially structs made up of classes. Components
  are not supposed to have any logic, but a small exception
  is done here to be able to manage them from the entities
  more easily
*/

#pragma once
#include "Animation.hpp"
#include "Vec2f.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>

class Component {
public:
  bool has = false;
};

class CTransform : public Component {
public:
  Vec2f pos = {0.0, 0.0};
  Vec2f prevPos = {0.0, 0.0};
  Vec2f scale = {1.0, 1.0};
  Vec2f vel = {0.0, 0.0};
  float angle = 0.0;

  CTransform() {};
  CTransform(const Vec2f &p) : pos(p) {};
};

class CState : public Component{
public:
  std::string state = "default";

  CState() {};
  CState(const std::string &s) : state(s) {};
};

class CLifespan : public Component{
public:
  size_t frames = 0;
  size_t remaining = 0;

  CLifespan() {};
  CLifespan(size_t frames) : frames(frames), remaining(frames) {};
};

class CInput : public Component {
public:
  bool up = false;
  bool down = false;
  bool right = false;
  bool left = false;
  bool prevJump = false; // Just to detect switching states with jump
  bool jump = false;
  bool shoot = false;
  bool canShoot = true;

  CInput() {};
};

class CBoxCollider : public Component {
public:
  Vec2f size;
  Vec2f halfSize;
  Vec2f offset = {0.0f, 0.0f};
  bool colliding = false;

  CBoxCollider() {};
  CBoxCollider(const Vec2f &size) : size(size), halfSize(size / 2), offset(size / 2) {};
  CBoxCollider(const Vec2f &size, const Vec2f &offset) : size(size), halfSize(size / 2), offset(offset) {};
};

class CAnimation : public Component {
public:
  std::shared_ptr<Animation> animation;

  CAnimation() {};
  CAnimation(std::shared_ptr<Animation> anim) : animation(anim) {};
};

class CGravity : public Component {
public:
  float speed = 0.0;

  CGravity() {};
  CGravity(float spd) : speed(spd) {};
};

class CTextBox : public Component {
public:
  std::shared_ptr<sf::Text> text;
  sf::RectangleShape box;
  sf::Color boxLineColor = { 128, 128, 128 };
  sf::Color boxFillColor = { 32, 32, 32, 192 };
  int boxLineSize = 1;
  int textSize = 15;

  CTextBox() {};
  CTextBox(std::shared_ptr<sf::Font> font, const std::string &txt) {
    text = std::make_shared<sf::Text>(*font, txt, textSize);
    text->setCharacterSize(textSize);
    box.setFillColor(boxFillColor);
    box.setOutlineColor(boxLineColor);
    box.setOutlineThickness(boxLineSize);
    box.setSize(sf::Vector2f(text->getLocalBounds().size.x + 20, text->getLocalBounds().size.y + 20));
  }

  CTextBox(std::shared_ptr<sf::Font> font, const std::string &txt, int size) {
    textSize = size;
    text = std::make_shared<sf::Text>(*font, txt, textSize);
    text->setCharacterSize(textSize);
    box.setFillColor(boxFillColor);
    box.setOutlineColor(boxLineColor);
    box.setOutlineThickness(boxLineSize);
    box.setSize(sf::Vector2f(text->getLocalBounds().size.x + 20, text->getLocalBounds().size.y + 20));
  }
};

class CEventTimer : public Component {
public:
  std::string name = "default";
  size_t total;
  size_t remaining;

  CEventTimer() {};
  CEventTimer(size_t frames) : total(frames), remaining(frames) {};
  CEventTimer(size_t frames, const std::string &n) : total(frames), remaining(frames), name(n) {};
};