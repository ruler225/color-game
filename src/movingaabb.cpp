#include "header/movingaabb.hpp"
#include "header/aabb.hpp"
#include "header/game.hpp"
#include "header/gameController.hpp"
#include "maths.cpp"
#include <algorithm>
#include <cmath>
#include <limits>

MovingAABB::MovingAABB() : AABB::AABB() { velocity = {0, 0, 0}; }

MovingAABB::MovingAABB(Vector3 minVec, Vector3 maxVec, Vector3 pos, Color c)
  : AABB(minVec, maxVec, pos, c) {
  velocity = {0, 0, 0};
}

void MovingAABB::update(int dt) {
  // only updates if the colour matches the game colour
  if (color == GameController::getInstance().getColor())
    pureUpdate(dt);
}

void MovingAABB::pureUpdate(int dt) {
  pos = add(pos, scalar_multiply(velocity, (dt / 1000.0f)));
}

void MovingAABB::accelerate(float x, float y, float z) {
  velocity[0] += x;
  velocity[1] += y;
  velocity[2] += z;
}

void MovingAABB::setVelocity(Vector3 newVelocity) { velocity = newVelocity; }

void MovingAABB::resolveCollision(AABB &obj) {

  //////////  Get minimum distance required to move out of collider ////////////
  float minMin, minMax, maxMin, maxMax;

  minMin = getDistForMin(getMin(), obj.getMin(), velocity);
  maxMin = getDistForMin(getMax(), obj.getMin(), velocity);
  minMax = getDistForMax(getMin(), obj.getMax(), velocity);
  maxMax = getDistForMax(getMax(), obj.getMax(), velocity);

  // get distance
  float dist = std::min(minMin, std::min(minMax, std::min(maxMin, maxMax)));

  ////////////////////////// Move outside of collider //////////////////////////
  Vector3 negVel{-velocity[0], -velocity[1], -velocity[2]};

  // normalise movement vector

  negVel = scalar_multiply(negVel, dist);

  // actually move outside of collider
  pos = add(pos, negVel);

  ////////////////////////////////  get normal  ////////////////////////////////
  Vector3 norm;
  if (getMin()[0] - obj.getMax()[0] >= -std::numeric_limits<float>::epsilon())
    norm = {1, 0, 0};
  else if (getMin()[1] - obj.getMax()[1] >=
           -std::numeric_limits<float>::epsilon())
    norm = {0, 1, 0};
  else if (getMin()[2] - obj.getMax()[2] >=
           -std::numeric_limits<float>::epsilon())
    norm = {0, 0, 1};
  else if (obj.getMin()[0] - getMax()[0] >=
           -std::numeric_limits<float>::epsilon())
    norm = {1, 0, 0};
  else if (obj.getMin()[1] - getMax()[1] >=
           -std::numeric_limits<float>::epsilon())
    norm = {0, 1, 0};
  else if (obj.getMin()[2] - getMax()[2] >=
           -std::numeric_limits<float>::epsilon())
    norm = {0, 0, 1};

  negVel = scalar_multiply(negVel, -1);

  //////////////////////////// project onto normal  ////////////////////////////
  Vector3 temp = project_onto(negVel, norm);

  //////////////////////////// subtract projection  ////////////////////////////
  negVel = subtract(negVel, temp);

  //////////////////////// move along resultant vector  ////////////////////////
  pos = add(pos, negVel);

  //////////////////  Subtract cancelled velocity component ////////////////////
  temp = project_onto(velocity, norm);

  velocity = subtract(velocity, temp);
}

float getDistForMin(Vector3 currentPoint, Vector3 minCollider, Vector3 velocity) {

  if (currentPoint[0] < minCollider[0] ||
      currentPoint[1] < minCollider[1] ||
      currentPoint[2] < minCollider[2])
    // return some garbage value to ignore
    return INFINITY;

  float temp[3];
  temp[0] = minCollider[0] - currentPoint[0];
  temp[1] = minCollider[1] - currentPoint[1];
  temp[2] = minCollider[2] - currentPoint[2];

  float tVel[3] = {-velocity[0], -velocity[1], -velocity[2]};

  float x = temp[0] / tVel[0];
  x = x > 0 ? x : INFINITY;
  float y = temp[1] / tVel[1];
  y = y > 0 ? y : INFINITY;
  float z = temp[2] / tVel[2];
  z = z > 0 ? z : INFINITY;

  return std::min(x, std::min(y, z));
}
float getDistForMax(Vector3 currentPoint, Vector3 maxCollider, Vector3 velocity) {
  if (currentPoint[0] > maxCollider[0] ||
      currentPoint[1] > maxCollider[1] ||
      currentPoint[2] > maxCollider[2])
    return INFINITY;

          float temp[3];
  temp[0] = maxCollider[0] - currentPoint[0];
  temp[1] = maxCollider[1] - currentPoint[1];
  temp[2] = maxCollider[2] - currentPoint[2];

  float tVel[3] = {-velocity[0], -velocity[1], -velocity[2]};

  float x = temp[0] / tVel[0];
  x = x > 0 ? x : INFINITY;
  float y = temp[1] / tVel[1];
  y = y > 0 ? y : INFINITY;
  float z = temp[2] / tVel[2];
  z = z > 0 ? z : INFINITY;

  return std::min(x, std::min(y, z));
}

////////////////////////////// Utility Functions  //////////////////////////////
Vector3 getForwardAcc(float theta, float phi, float mag, int dt) {
  float fMag = ((float)dt / 1000.0f) * mag;
  Vector3 forward;
  forward[0] = sin(theta) * fMag;
  forward[1] = 0;
  forward[2] = cos(theta) * fMag;
  return forward;
}

Vector3 getRightAcc(float theta, float phi, float mag, int dt) {
  float fMag = ((float)dt / 1000.0f) * mag;
  Vector3 side;
  side[0] = sin(theta - M_PI / 2) * fMag;
  side[1] = 0;
  side[2] = cos(theta - M_PI / 2) * fMag;
  return side;
}
