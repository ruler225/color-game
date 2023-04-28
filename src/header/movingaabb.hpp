#pragma once

#include "aabb.hpp"
#include "game.hpp"

class MovingAABB : public AABB {
private:
  float getMinDistance(Vector3 a, Vector3 b, Vector3 v);

  protected:
    Vector3 velocity;

public:
  MovingAABB();   //Initialize MovingAABB with velocity of (0, 0, 0) and (0, 0, 0) min/max vectors
  MovingAABB(Vector3 minVec, Vector3 maxVec, Vector3 pos);

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
  virtual void resolveCollision(AABB& obj);
};
