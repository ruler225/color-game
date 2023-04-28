#pragma once

#include "aabb.hpp"
#include "game.hpp"

class MovingAABB : public AABB {
private:
  float getMinDistance(Vector3 a, Vector3 b, Vector3 v);

protected:
  Vector3 velocity;

  void pureUpdate(int);

public:
  MovingAABB(); // Initialize MovingAABB with velocity of (0, 0, 0) and (0, 0,
                // 0) min/max vectors
  MovingAABB(Vector3 minVec, Vector3 maxVec, Vector3 pos, Color c = RED);

  /**
   * @brief      Updates the state of the AABB
   *
   * @details    Updates the position of the entity
   *
   * @param      t: int = Time since last called in millisecond
   *
   * @return     void
   */
  virtual void update(int timeDelta);
  void accelerate(float x, float y, float z);
  void setVelocity(Vector3 newVelocity);
  virtual void resolveCollision(AABB &obj);
};

float getDistForMin(Vector3 currentPoint, Vector3 minCollider, Vector3 v);
float getDistForMax(Vector3 currentPoint, Vector3 maxCollider, Vector3 v);
Vector3 getForwardAcc(float theta, float delta, float mag, int dt);
Vector3 getRightAcc(float theta, float delta, float mag, int dt);
