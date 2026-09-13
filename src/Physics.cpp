#include "Physics.hpp"

Vec2f Physics::getOverlap(std::shared_ptr<Entity> obj, std::shared_ptr<Entity> other){
  Vec2f overlap = {0, 0};
  Vec2f objPos = obj->getComponent<CTransform>().pos;
  //objPos -= obj->getComponent<CBoxCollider>().offset;
  Vec2f objColHalfSize = obj->getComponent<CBoxCollider>().halfSize;

  Vec2f otherPos = other->getComponent<CTransform>().pos;
  //otherPos -= other->getComponent<CBoxCollider>().offset;  
  Vec2f otherColHalfSize = other->getComponent<CBoxCollider>().halfSize;

  float dx = std::abs(otherPos.x - objPos.x);
  overlap.x = (otherColHalfSize.x + objColHalfSize.x) - dx;

  float dy = std::abs(otherPos.y - objPos.y);
  overlap.y = (otherColHalfSize.y + objColHalfSize.y) - dy;

  return overlap;
}

Vec2f Physics::getPreviousOverlap(std::shared_ptr<Entity> obj, std::shared_ptr<Entity> other) {
  Vec2f overlap = {0, 0};
  Vec2f objPos = obj->getComponent<CTransform>().prevPos;
  //objPos -= obj->getComponent<CBoxCollider>().offset;  
  Vec2f objColHalfSize = obj->getComponent<CBoxCollider>().halfSize;

  Vec2f otherPos = other->getComponent<CTransform>().prevPos;
  //otherPos -= other->getComponent<CBoxCollider>().offset;    
  Vec2f otherColHalfSize = other->getComponent<CBoxCollider>().halfSize;

  float dx = std::abs(otherPos.x - objPos.x);
  overlap.x = (otherColHalfSize.x + objColHalfSize.x) - dx;

  float dy = std::abs(otherPos.y - objPos.y);
  overlap.y = (otherColHalfSize.y + objColHalfSize.y) - dy;

  return overlap;
}
