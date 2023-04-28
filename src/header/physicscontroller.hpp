#pragma once

#include "aabbbranch.hpp"
#include "event_collider.hpp"
#include "game.hpp"
#include "movingaabb.hpp"
#include <vector>

constexpr float GRAVITY_CONST = 10.0;

class PhysicsController {
private:
  // The AABBBranch root node, resembles the whole tree.
  AABBBranch &tree;
  std::vector<EventCollider*> eventColliders;

  // The MovingAABB objects that should be regularly updated
  std::vector<MovingAABB *> registeredObjects;
  // The MovingAABB objects that should have gravity applied to them
  std::vector<MovingAABB *> gravityRegisteredObjects;

  /**
   * @brief      Constructs a new PhysicsController object
   *
   * @details    Constructs a new PhysicsController object and initialises the
   * AABBBranch& tree
   *
   * @param      void
   *
   * @return     PhysicsController
   */
  PhysicsController();

public:
  // Singleton getter
  static PhysicsController &getInstance();

  /**
   * @brief      Update all moving physics objects
   *
   * @details    Applies gravity to all gravity objects, calls
   *  `update(timeDelta)` on all moving physics objects(MovingAABB), then
   * resolves collision on all moving physics objects by calling
   * `resolveCollision(AABB&)` on all moving physics objects with all AABBs it
   * is colliding with. It gets all colliding AABBs using `tree.getColliders`.
   *
   * @param      timeDelta: int = time since last call in milliseconds
   *
   * @return     void
   */
  void updateAll(int timeDelta);

  /**
   * @brief      Registers AABB object
   *
   * @details    Registers AABB object in the AABB tree
   *
   * @param      obj: AABB* = A pointer to the AABB object
   *
   * @return     void
   */
  void registerObject(AABB *obj);

  /**
   * @brief      Registers MovingAABB objects to the AABB tree
   *
   * @details    If isGravityAffected is true, then the object will be added to
   * the list of gravity affected objects
   *
   * @param      obj: MovingAABB* = A pointer to the object tor register
   *
   * @param      isGravityAffected: bool = A boolean to indicate if the object is affected by gravity
   *
   * @return     void
   */
  void registerObject(MovingAABB *obj, bool isGravityAffected);

  void registerObject(EventCollider *eCollider);

  /**
   * @brief      Removes a MovingAABB from the list of registered objects
   *
   * @param      obj: AABB* = object to deregister
   *
   * @return     void
   */
  void deregisterObject(AABB *obj);
  /**
   * @brief      Removes a MovingAABB from the list of registered objects
   *
   * @param      obj: MovingAABB* = object to deregister
   *
   * @return     void
   */
  void deregisterObject(MovingAABB *obj);

  /**
   * @brief      Checks for collision with all registered physics objects
   *
   * @details    detailed description
   *
   * @param      obj: AABB& = The object to check collision for
   *
   * @return     boolean
   */
  bool isColliding(AABB &obj);
};
