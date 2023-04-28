#include "header/aabbbranch.hpp"
#include "header/aabb.hpp"
#include<typeinfo>
#include<math.h>
#include <vector>

AABBBranch::AABBBranch(): AABB::AABB() {
    left = NULL;
    right = NULL;
}

AABBBranch::AABBBranch(AABB *leftObj, AABB *rightObj): AABB::AABB(), left(leftObj), right(NULL) {

    //Check if one of them is null
    if (leftObj && rightObj) {  //Set the min and max vectors so that they cover both children
        min[0] = std::min(leftObj->getMin()[0], rightObj->getMin()[0]);
        min[1] = std::min(leftObj->getMin()[1], rightObj->getMin()[1]);
        min[2] = std::min(leftObj->getMin()[2], rightObj->getMin()[2]);
        max[0] = std::max(leftObj->getMax()[0], rightObj->getMax()[0]);
        max[1] = std::max(leftObj->getMax()[1], rightObj->getMax()[1]);
        max[2] = std::max(leftObj->getMax()[2], rightObj->getMax()[2]);
    } else if (!leftObj && rightObj) {
        min = rightObj->getMin();    //Since we're using std::array, this should actually copy the array (I hope)
        max = rightObj->getMax();
    } else if (leftObj && !rightObj) {
        min = leftObj->getMin();
        max = leftObj->getMax();
    } else {
        left = NULL;
        right = NULL;
    }
}

void AABBBranch::addToLeft(AABB *obj) {
    //If the child is a node, create a new branch and add both nodes to the new branch
    if (typeid(left) != typeid(AABBBranch)) {
        AABB *oldNode = left;
        AABBBranch *newBranch = new AABBBranch(oldNode, obj);
        left = newBranch;
    } else {
        ((AABBBranch*) left)->add(obj);
    }
}

void AABBBranch::addToRight(AABB *obj) {
    //If the child is a node, create a new branch and add both nodes to the new branch
    if (typeid(right) != typeid(AABBBranch)) {
        AABB *oldNode = right;
        AABBBranch *newBranch = new AABBBranch(oldNode, obj);
        right = newBranch;
    } else {
        ((AABBBranch*) right)->add(obj);
    }
}

void AABBBranch::updateBounds() {
    if (!left && right) {
        min = right->getMin();
        max = right->getMax();
    } else if (left && !right) {
        min = left->getMin();
        max = left->getMax();
    } else if (!left && !right) {
        min = {0, 0, 0};
        max = {0, 0, 0};
    } else {
        min[0] = std::min(left->getMin()[0], right->getMin()[0]);
        min[1] = std::min(left->getMin()[1], right->getMin()[1]);
        min[2] = std::min(left->getMin()[2], right->getMin()[2]);
        max[0] = std::max(left->getMax()[0], right->getMax()[0]);
        max[1] = std::max(left->getMax()[1], right->getMax()[1]);
        max[2] = std::max(left->getMax()[2], right->getMax()[2]);
    }
}


//NOTE: at the moment this is implemented recursively. This should be revised at some point to just use a loop
void AABBBranch::add(AABB *obj) {
    //Case 0: There are currently no AABB entities in the tree (empty branch)
    if (!left && !right) {
        left = obj;
    }
    //Case 1: The branch has one child
    else if (!left && right) {
        //Case 1a: The child has no intersection with given entity
        if (!right->isColliding(*obj)) {
            left = obj;
        }
        //Case 1b: The child does intersect with given entity (add it to that branch)
        else {
            addToRight(obj);
        }

    } else if (left && !right) { //Same thing as case 1 but for empty right child
        //Case 1a: The child has no intersection with given entity
        if (!left->isColliding(*obj)) {
            right = obj;
        }
        //Case 1b: The child does intersect with given entity (add it to that branch)
        else {
            addToLeft(obj);
        }
    }
    //Case 2: The branch has two children
    else {
        //Case 2a: The given entity collides with one of the branches
        if (left->isColliding(*obj)) {
            addToLeft(obj);
        } else if (right->isColliding(*obj)) {
            addToRight(obj);
        } 
        //Case 2b: Entity collides with neither child
        else {
            /* Check which branch the object should be added to by calculating the new area for both
            and adding it to the branch with the least resulting area
            */
            Vector3 newMax = {0, 0, 0};
            Vector3 newMin = {0, 0, 0};
           //Compute new left area
            newMin[0] = std::min(obj->getMin()[0], right->getMin()[0]);
            newMin[1] = std::min(obj->getMin()[1], right->getMin()[1]);
            newMin[2] = std::min(obj->getMin()[2], right->getMin()[2]);
            newMax[0] = std::max(obj->getMax()[0], right->getMax()[0]);
            newMax[1] = std::max(obj->getMax()[1], right->getMax()[1]);
            newMax[2] = std::max(obj->getMax()[2], right->getMax()[2]);
            float leftArea = abs((newMax[0] - newMin[0])*(newMax[1] - newMin[1])*(newMax[2] - newMin[2]));
            //Compute new right area
            newMin[0] = std::min(left->getMin()[0], obj->getMin()[0]);
            newMin[1] = std::min(left->getMin()[1], obj->getMin()[1]);
            newMin[2] = std::min(left->getMin()[2], obj->getMin()[2]);
            newMax[0] = std::max(left->getMax()[0], obj->getMax()[0]);
            newMax[1] = std::max(left->getMax()[1], obj->getMax()[1]);
            newMax[2] = std::max(left->getMax()[2], obj->getMax()[2]);
            float rightArea = abs((newMax[0] - newMin[0])*(newMax[1] - newMin[1])*(newMax[2] - newMin[2]));
            if (leftArea <= rightArea) {
                addToLeft(obj);
            } else {
                addToRight(obj);
            }
        }
    }

    //Update min and max vectors for current branch
    this->updateBounds();
}

//Just like with add, this is implemented recursively. This may change in the future
void AABBBranch::remove(AABB *obj) {
    //Traverse the tree until the specified object is found
    if (typeid(left) != typeid(AABBBranch)) {
        if (obj == left) {
            left = NULL;
            this->updateBounds();
            return;
        }
    }
    if (typeid(right) != typeid(AABBBranch)) {
        if (obj == right) {
            right = NULL;
            this->updateBounds();
            return;
        }
    }
    //Gotta check for null pointers before doing anything
    if (typeid(left) == typeid(AABBBranch)) {
        if (left->isColliding(*obj)) {
            ((AABBBranch*) left)->remove(obj);
            this->updateBounds();
            return;
        } 
    }
    if (typeid(right) == typeid(AABBBranch))
        if (right->isColliding(*obj)) {
            ((AABBBranch*) right)->remove(obj);
            this->updateBounds();
        }
}

//Updates an object in the tree by removing the object from the tree and reinserting it
void AABBBranch::update(AABB *obj) {
    this->remove(obj);
    this->add(obj);
}

//TODO: improve space efficiency, creates a new vector object for each call
std::vector<AABB *> AABBBranch::getColliders(AABB &obj) {
  std::vector<AABB *> colliders;

  if (obj.isColliding(*this->left)) {
    if (AABBBranch *b = dynamic_cast<AABBBranch*>(this->left)){
      std::vector<AABB*> leftColliders(b->getColliders(obj));
      colliders.insert(colliders.cbegin(), leftColliders.cbegin(),
                       leftColliders.cend());
    }
    else
      colliders.push_back(this->left);
  }
  if (obj.isColliding(*this->right)) {
    if (AABBBranch *b = dynamic_cast<AABBBranch *>(this->right)) {
      std::vector<AABB*> rightColliders(b->getColliders(obj));
      colliders.insert(colliders.cbegin(), rightColliders.cbegin(),
                       rightColliders.cend());
    }
     else
      colliders.push_back(this->right);
  }

  return colliders;
}

bool AABBBranch::hasCollider(AABB *obj) {
    if (left && typeid(left) != typeid(AABBBranch)) {
        if (left->isColliding(*obj)) {
            return true;
        }
    }
    if (right && typeid(right) != typeid(AABBBranch)) {
        if (right->isColliding(*obj)) {
            return true;
        }
    }
    if (typeid(left) == typeid(AABBBranch)) {
        if (left->isColliding(*obj)) {
            return ((AABBBranch*) left)->hasCollider(obj);
        }
    }
    if (typeid(right) == typeid(AABBBranch)) {
        if (right->isColliding(*obj)) {
            return ((AABBBranch*) right)->hasCollider(obj);
        }
    }

    return false;

}
