#pragma once

#include "game.hpp"
#include "aabbbranch.hpp"
#include "movingaabb.hpp"
#include <vector>

class PhysicsController {
    private:
    //Singleton instance
    static PhysicsController *instance;

    AABBBranch *tree;

    //The MovingAABB objects that should be regularly updated
    std::vector<MovingAABB*> registeredObjects;
    //The MovingAABB objects that should have gravity applied to them
    std::vector<MovingAABB*> gravityRegisteredObjects;

    PhysicsController();

    public:

    //Singleton getter
    static PhysicsController* getInstance();

    //Update all MovingAABB objects registered for physics
    void updateAll(int timeDelta);

    //If isGravityAffected is true, then the object will be added to the list of gravity affected objects
    void registerObject(AABB *obj);
    void registerObject(MovingAABB *obj, bool isGravityAffected);
    //Removes a MovingAABB from the list of registered objects
    void deregisterObject(AABB *obj);
    void deregisterObject(MovingAABB *obj);

    bool hasCollider(AABB &obj);

};