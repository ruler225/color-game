#include "header/aabb.hpp"
#include "header/gameController.hpp"
#include <math.h>
#include <algorithm>
#include <cstdlib>

AABB::AABB(Vector3 minVec, Vector3 maxVec, Vector3 pos, Color c)
  : min(minVec), max(maxVec), color(c), pos(pos) {}

AABB::AABB() { pos = max = min = {0, 0, 0}; }

float AABB::area() {
  return abs((max[0] - min[0]) * (max[1] - min[1]) * (max[2] - min[2]));
}

bool AABB::isColliding(AABB &obj) {
  if (color == GameController::getInstance().getColor())
    return pureCollision(obj);
  return false;
}

bool AABB::pureCollision(AABB &obj) {
  return obj.getMax()[0] > this->getMin()[0] && obj.getMin()[0] < this->getMax()[0] &&
         obj.getMax()[1] > this->getMin()[1] && obj.getMin()[1] < this->getMax()[1] &&
         obj.getMax()[2] > this->getMin()[2] && obj.getMin()[2] < this->getMax()[2];
}

Color AABB::getColor() { return color; }

Vector3 AABB::getMin() {
  return Vector3{min[0] + pos[0], min[1] + pos[1],
                 min[2] + pos[2]};
}
Vector3 AABB::getMax() {
  return Vector3{AABB::max[0] + pos[0], AABB::max[1] + pos[1],
                 AABB::max[2] + pos[2]};
}
