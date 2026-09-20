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
