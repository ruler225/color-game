#pragma once

#include "aabb.hpp"
#include <vector>

class AABBBranch : public AABB {
    AABB *left;
    AABB *right;

    void addToLeft(AABB *obj);
    void addToRight(AABB *obj);
    void updateBounds();

    public:
    AABBBranch(); //Initialize a branch with two null children
    AABBBranch(AABB *leftObj, AABB *rightObj); //Initialize a branch with two specified children

    void add(AABB *obj);
    void remove(AABB *obj);
    void update(AABB *obj); //Update a specified object in the branch

  /**
   * @brief      Get all colliders with obj
   *
   * @details    Returns a vector of AABB objects that collide with obj
   *
   * @param      obj&: Reference to AABB = The object to get the vector of colliders for
   *
   * @return     std::vector<AABB*>
   */
  std::vector<AABB*> getColliders(AABB &obj);
  bool hasCollider(AABB *obj);

};
