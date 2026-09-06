/*
  Physics.hpp
  Main class to handle various physics calls like collisions.
*/

#pragma once
#include "Vec2f.hpp"
#include "Entity.hpp"

class Physics {

public:
  Vec2f getOverlap(std::shared_ptr<Entity> obj, std::shared_ptr<Entity> other);
  Vec2f getPreviousOverlap(std::shared_ptr<Entity> obj, std::shared_ptr<Entity> other);
};
