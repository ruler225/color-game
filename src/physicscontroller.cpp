#include "header/physicscontroller.hpp"
#include "header/game.hpp"
#include<algorithm>

PhysicsController *PhysicsController::instance = nullptr;

PhysicsController::PhysicsController() {
    AABBBranch();
    registeredObjects = std::vector<MovingAABB*>();
    gravityRegisteredObjects = std::vector<MovingAABB*>();
}

PhysicsController* PhysicsController::getInstance() {
    if (instance != nullptr) {
        instance = new PhysicsController();
    }
    return instance;
}

void PhysicsController::updateAll(int timeDelta) {
    //Iterate through gravity-registered objects and apply gravity constant to each velocity
    float gravityChange = GRAVITY_CONST/1000.0;
    for (int i = 0; i < gravityRegisteredObjects.size(); i++) {
        gravityRegisteredObjects[i]->accelerate(0, -((gravityChange*timeDelta)/1000.0), 0);
    }
    /*NOTE: it is assumed that the resolveCollision method will also reset each object's velocity when colliding,
    so that each gravity registered object will not have its downwards acceleration infinitely increased
    */

    //Update all entities registered for physics
    for (int i = 0; i < registeredObjects.size(); i++) {
        registeredObjects[i]->update(timeDelta);
        //Get all objects colliding with current entity and resolve each collision
        std::vector<AABB*> colliders = tree->getColliders(*registeredObjects[i]);
        for (int j = 0; j < colliders.size(); j++) {
            registeredObjects[i]->resolveCollision(*colliders[j]);
        }
        tree->update(registeredObjects[i]);
    }

}

void PhysicsController::registerObject(AABB *obj) {
    //Add object to the tree
    tree->add(obj);
}

void PhysicsController::registerObject(MovingAABB *obj, bool isGravityAffected) {
    //Check if object is already registered
    if (std::find(registeredObjects.begin(), registeredObjects.end(), obj) == registeredObjects.end())
        return;

    if (isGravityAffected)
        gravityRegisteredObjects.push_back(obj);
    registeredObjects.push_back(obj);

    tree->add(obj);
}


void PhysicsController::deregisterObject(AABB *obj) {
    tree->remove(obj);
}

void PhysicsController::deregisterObject(MovingAABB *obj) {
    //Remove object from gravity registered objects, if it exists there
    auto gravityIterator = std::find(gravityRegisteredObjects.begin(), gravityRegisteredObjects.end(), obj);
    if (gravityIterator != gravityRegisteredObjects.end())
        gravityRegisteredObjects.erase(gravityIterator);
    
    auto registeredIterator = std::find(registeredObjects.begin(), registeredObjects.end(), obj);
    if (registeredIterator != registeredObjects.end()) {
        registeredObjects.erase(registeredIterator);
        tree->remove(obj);
    }
}

bool PhysicsController::hasCollider(AABB &obj) {
    return tree->hasCollider(&obj);
}
