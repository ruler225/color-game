#include "header/deadly_platform.hpp"

bool DeadlyPlatform::isColliding(AABB obj){
  bool isCol = AABB::isColliding(obj);

  // check if collider is instance of player

  return isCol;
}
