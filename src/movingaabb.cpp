#include "header/movingaabb.hpp"
#include "header/aabb.hpp"
#include "header/game.hpp"
#include <algorithm>
#include <cmath>

MovingAABB::MovingAABB(): AABB::AABB() {
    velocity = {0, 0, 0};
}

MovingAABB::MovingAABB(Vector3 minVec, Vector3 maxVec, Vector3 pos)
    : AABB(minVec, maxVec, pos) {
  velocity = {0, 0, 0};
}

void MovingAABB::update(int dt) {
  pos[0] += velocity[0] * (dt / 1000.0f);
  pos[1] += velocity[1] * (dt / 1000.0f);
  pos[2] += velocity[2] * (dt / 1000.0f);
}

void MovingAABB::accelerate(float x, float y, float z) {
    velocity[0] += x;
    velocity[1] += y;
    velocity[2] += z;
}

void MovingAABB::setVelocity(Vector3 newVelocity) {
    velocity = newVelocity;
}

void MovingAABB::resolveCollision(AABB &obj) {

  //////////  Get minimum distance required to move out of collider ////////////
  float minMin = getMinDistance(this->getMin(), obj.getMin(), this->velocity);
  float minMax = getMinDistance(this->getMin(), obj.getMax(), this->velocity);
  float maxMin = getMinDistance(this->getMax(), obj.getMin(), this->velocity);
  float maxMax = getMinDistance(this->getMax(), obj.getMax(), this->velocity);

  // get distance
  float dist = std::min(minMin, std::min(minMax, std::min(maxMin, maxMax)));

  ////////////////////////// Move outside of collider //////////////////////////
  Vector3 negVel{-velocity[0], -velocity[1], -velocity[2]};

  // normalise movement vector
  float negMag = std::sqrt(negVel[0] * negVel[0] + negVel[2] * negVel[1] +
                           negVel[2] * negVel[2]);
  negVel[0] /= negMag;
  negVel[1] /= negMag;
  negVel[2] /= negMag;

  negVel[0] *= dist;
  negVel[1] *= dist;
  negVel[2] *= dist;

  // actually move outside of collider
  pos[0] += negVel[0];
  pos[1] += negVel[1];
  pos[2] += negVel[2];

  ////////////////////////////////  get normal  ////////////////////////////////
  Vector3 norm;
  //TODO: use a more float friendly comparison
  if (getMin()[0] - obj.getMax()[0] >= 0)
    norm = {1, 0, 0};
  else if (getMin()[1] - obj.getMax()[1] >= 0)
    norm = {0, 1, 0};
  else if (getMin()[2] - obj.getMax()[2] >= 0)
    norm = {0, 0, 1};
  else if (getMax()[0] - obj.getMin()[0] <= 0)
    norm = {-1, 0, 0};
  else if (getMax()[1] - obj.getMin()[1] <= 0)
    norm = {0, -1, 0};
  else if (getMax()[2] - obj.getMin()[2] <= 0)
    norm = {0, 0, -1};

  negVel[0] = -negVel[0];
  negVel[1] = -negVel[1];
  negVel[2] = -negVel[2];

  //////////////////////////// project onto normal  ////////////////////////////
  float dot = negVel[0] * norm[0] + negVel[1] * norm[1] + // get lost vector
              negVel[2] * norm[2];
  norm[0] *= dot;
  norm[1] *= dot;
  norm[2] *= dot;

  //////////////////////////// subtract projection  ////////////////////////////
  negVel[0] -= norm[0];
  negVel[1] -= norm[1];
  negVel[2] -= norm[2];

  //////////////////////// move along resultant vector  ////////////////////////
  pos[0] += negVel[0];
  pos[1] += negVel[1];
  pos[2] += negVel[2];
}

float MovingAABB::getMinDistance(Vector3 a, Vector3 b, Vector3 v) {
  float tVel[3] = {-v[0], -v[1], -v[2]};

  float temp[3];
  temp[0] = b[0] - a[0];
  temp[1] = b[1] - a[1];
  temp[2] = b[2] - a[2];

  float x = temp[0] / tVel[0];
  float y = temp[1] / tVel[1];
  float z = temp[2] / tVel[2];

  return std::max(x, std::max(y, z));
}
