#include "header/aabb.hpp"
#include "header/game.hpp"
#include<math.h>

AABB::AABB(Vector3 minVec, Vector3 maxVec, Vector3 pos, Color c): min(minVec), max(maxVec), color(c) {}
 AABB::AABB(Vector3 minVec, Vector3 maxVec, Vector3 pos): min(minVec), max(maxVec), pos(pos) {color = RED;}

AABB::AABB() {
    Vector3 minVec, maxVec = {0, 0, 0};
    min = minVec;
    max = maxVec;
    pos = {0, 0, 0};
}

float AABB::area() {
    return abs((max[0] - min[0])*(max[1] - min[1])*(max[2] - min[2]));
}

bool AABB::isColliding(AABB &obj) {
    return obj.max[0] > this->min[0] && obj.min[0] < this->max[0] &&
            obj.max[1] > this->min[1] && obj.min[1] < this->max[1] &&
            obj.max[2] > this->min[2] && obj.min[2] < this->max[2];
}

Color AABB::getColor() { return color; }

Vector3 AABB::getMin() {
  return Vector3{getMin()[0] + pos[0], getMin()[1] + pos[1],
                 getMin()[2] + pos[2]};
}
Vector3 AABB::getMax() {
  return Vector3{AABB::getMax()[0] + pos[0], AABB::getMax()[1] + pos[1],
                 AABB::getMax()[2] + pos[2]};
}
