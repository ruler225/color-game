#include "header/physicscontroller.hpp"
#include "header/aabbbranch.hpp"
#include "header/event_collider.hpp"
#include "header/game.hpp"
#include "header/gameController.hpp"
#include "header/movingaabb.hpp"
#include "header/player.hpp"
#include <algorithm>
#include <vector>

using std::vector;

Player &player = GameController::getInstance().getPlayer();

PhysicsController::PhysicsController() : tree{*(new AABBBranch())} {}

PhysicsController& PhysicsController::getInstance() {
  static PhysicsController instance;
  return instance;
}

// Iterate through gravity-registered objects and apply gravity constant to each
// velocity
void PhysicsController::updateAll(int timeDelta) {
  float gravityChange = timeDelta * GRAVITY_CONST / 1000.0;

  ////////////////  Add gravity acceleration to moving objects  ////////////////
  // handle player
  player.accelerate(0, -gravityChange, 0);

  // handle other MovingAABB
  for (MovingAABB *mAABB : gravityRegisteredObjects)
    mAABB->accelerate(0, -gravityChange, 0);

  ////////////////  Update all entities registered for physics  ////////////////
  // Handle player
  player.update(timeDelta);

  // kill player if falling through the map
  if (player.getMax()[1] <= -10)
    GameController::getInstance().killPlayer();

  vector<AABB *> colliders = tree.getColliders(player);
  for (AABB *collider : colliders)
    player.resolveCollision(*collider);

  // handle other MovingAABB
  for (MovingAABB *aabb : registeredObjects) {
    aabb->update(timeDelta);

    // Get all objects colliding with current entity and resolve each collision
    vector<AABB *> colliders = tree.getColliders(*aabb);
    for (AABB *collider : colliders)
      aabb->resolveCollision(*collider);
    tree.update(aabb);
  }

  //////////////////// Check event colliders for collision  ////////////////////
  for (EventCollider *eCollider : eventColliders)
    eCollider->isColliding(player);
}

void PhysicsController::registerObject(AABB *obj) { tree.add(obj); }

void PhysicsController::registerObject(MovingAABB *obj,
                                       bool isGravityAffected) {
  // Check if object is already registered
  if (std::find(registeredObjects.begin(), registeredObjects.end(), obj) !=
      registeredObjects.end())
    return;

  //////////////////////////// Register new Object  ////////////////////////////
  // add register object to appropriate lists
  if (isGravityAffected)
    gravityRegisteredObjects.push_back(obj);
  registeredObjects.push_back(obj);

  // register in AABB tree
  tree.add(obj);
}

void PhysicsController::registerObject(EventCollider* evCollider) {
  if (std::find(eventColliders.begin(), eventColliders.end(), evCollider) !=
      eventColliders.end())
    return;
  eventColliders.push_back(evCollider);
}

void PhysicsController::deregisterObject(AABB *obj) { tree.remove(obj); }

void PhysicsController::deregisterObject(MovingAABB *obj) {
  // Remove object from gravity registered objects, if it exists there
  auto gravityIterator = std::find(gravityRegisteredObjects.begin(),
                                   gravityRegisteredObjects.end(), obj);
  if (gravityIterator != gravityRegisteredObjects.end())
    gravityRegisteredObjects.erase(gravityIterator);

  // remove from objects list and AABB tree
  auto registeredIterator =
      std::find(registeredObjects.begin(), registeredObjects.end(), obj);
  if (registeredIterator != registeredObjects.end()) {
    registeredObjects.erase(registeredIterator);
    tree.remove(obj);
  }
}

bool PhysicsController::isColliding(AABB &obj) { return tree.isColliding(obj); }
