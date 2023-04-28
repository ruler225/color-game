#include "header/aabbbranch.hpp"
#include <math.h>
#include <stack>

using std::stack;
using std::vector;

AABBBranch::AABBBranch() : AABB::AABB() {
  left = nullptr;
  right = nullptr;
}

AABBBranch::AABBBranch(AABB *leftObj, AABB *rightObj)
    : AABB::AABB(), left(leftObj), right(rightObj) {

  // Check if one of them is null
  if (left != nullptr &&
      right != nullptr) { // Set the min and max vectors so that they cover
                             // both children
    min[0] = std::min(left->getMin()[0], right->getMin()[0]);
    min[1] = std::min(left->getMin()[1], right->getMin()[1]);
    min[2] = std::min(left->getMin()[2], right->getMin()[2]);
    max[0] = std::max(left->getMax()[0], right->getMax()[0]);
    max[1] = std::max(left->getMax()[1], right->getMax()[1]);
    max[2] = std::max(left->getMax()[2], right->getMax()[2]);
  } else if (left == nullptr && right != nullptr) {
    min = right->getMin();
    max = right->getMax();
  } else if (left != nullptr && right == nullptr) {
    min = left->getMin();
    max = left->getMax();
  }
}

void AABBBranch::addToLeft(AABB *obj) {
  // If the child is a node, create a new branch and add both nodes to the new
  // branch
  if (AABBBranch *branch = dynamic_cast<AABBBranch *>(left)){
    branch->add(obj);
  }
  else {
    AABBBranch *newBranch = new AABBBranch(left, obj);
    left = newBranch;
  }
}

void AABBBranch::addToRight(AABB *obj) {
  // If the child is a node, create a new branch and add both nodes to the new
  // branch
  if (AABBBranch *branch = dynamic_cast<AABBBranch *>(right)) {
    branch->add(obj);
  } else {
    AABBBranch *newBranch = new AABBBranch(right, obj);
    right = newBranch;
  }
}

void AABBBranch::updateBounds() {
  if (left == nullptr && right != nullptr) {
    min = right->getMin();
    max = right->getMax();
  } else if (left != nullptr && right == nullptr) {
    min = left->getMin();
    max = left->getMax();
  } else if (left == nullptr && right == nullptr) {
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

void AABBBranch::add(AABB *obj) {
  // Case 0: There are currently no AABB entities in the tree (empty branch)
  if (left == nullptr && right == nullptr) {
    left = obj;
  }
  // Case 1: The branch has one child
  else if (left == nullptr && right != nullptr) {
    // Case 1a: The child has no intersection with given entity
    if (!right->pureCollision(*obj)) {
      left = obj;
    }
    // Case 1b: The child does intersect with given entity (add it to that
    // branch)
    else {
      addToRight(obj);
    }

  } else if (left != nullptr && right == nullptr) { // Same thing as case 1 but for empty right child
    // Case 1a: The child has no intersection with given entity
    if (!left->pureCollision(*obj)) {
      right = obj;
    }
    // Case 1b: The child does intersect with given entity (add it to that
    // branch)
    else {
      addToLeft(obj);
    }
  }
  // Case 2: The branch has two children
  else {
    // Case 2a: The given entity collides with one of the branches
    if (left->pureCollision(*obj)) {
      addToLeft(obj);
    } else if (right->pureCollision(*obj)) {
      addToRight(obj);
    }
    // Case 2b: Entity collides with neither child
    else {
      /* Check which branch the object should be added to by calculating the new
      area for both and adding it to the branch with the least resulting area
      */
      Vector3 newMax = {0, 0, 0};
      Vector3 newMin = {0, 0, 0};
      // Compute new left area
      newMin[0] = std::min(obj->getMin()[0], right->getMin()[0]);
      newMin[1] = std::min(obj->getMin()[1], right->getMin()[1]);
      newMin[2] = std::min(obj->getMin()[2], right->getMin()[2]);
      newMax[0] = std::max(obj->getMax()[0], right->getMax()[0]);
      newMax[1] = std::max(obj->getMax()[1], right->getMax()[1]);
      newMax[2] = std::max(obj->getMax()[2], right->getMax()[2]);
      float leftArea = abs((newMax[0] - newMin[0]) * (newMax[1] - newMin[1]) *
                           (newMax[2] - newMin[2]));
      // Compute new right area
      newMin[0] = std::min(left->getMin()[0], obj->getMin()[0]);
      newMin[1] = std::min(left->getMin()[1], obj->getMin()[1]);
      newMin[2] = std::min(left->getMin()[2], obj->getMin()[2]);
      newMax[0] = std::max(left->getMax()[0], obj->getMax()[0]);
      newMax[1] = std::max(left->getMax()[1], obj->getMax()[1]);
      newMax[2] = std::max(left->getMax()[2], obj->getMax()[2]);
      float rightArea = abs((newMax[0] - newMin[0]) * (newMax[1] - newMin[1]) *
                            (newMax[2] - newMin[2]));
      if (leftArea <= rightArea) {
        addToLeft(obj);
      } else {
        addToRight(obj);
      }
    }
  }

  // Update min and max vectors for current branch
  this->updateBounds();
}

// Just like with add, this is implemented recursively. This may change in the
// future
void AABBBranch::remove(AABB *obj) {
  // Traverse the tree until the specified object is found

  ////////////////////  Check if object is one of branches  ////////////////////
  // Check left
  if (obj == left){
    left = nullptr;
    this->updateBounds();
    return;
  }

  // Check right
  if (obj == right){
    right = nullptr;
    this->updateBounds();
    return;
  }

  //////////////  Check if object is inside one of the branches ////////////////
  // Only works if the branches are of type AABBBranch

  // Check left
  if (left->pureCollision(*obj))
    if (AABBBranch *lBranch = dynamic_cast<AABBBranch *>(left)) {
      lBranch->remove(obj);
      this->updateBounds();
      return;
    }

  // Check right
  if (right->pureCollision(*obj))
    if (AABBBranch *rBranch = dynamic_cast<AABBBranch *>(right)){
      rBranch->remove(obj);
      this->updateBounds();
      return;
    }
}

// Updates an object in the tree by removing the object from the tree and
// reinserting it
void AABBBranch::update(AABB *obj) {
  this->remove(obj);
  this->add(obj);
}

vector<AABB *> AABBBranch::getColliders(AABB &obj) {

  vector<AABB *> colliders;
  stack<AABBBranch *> branchTodo;

  AABBBranch *currentBranch;
  branchTodo.push(this);

  while (!branchTodo.empty()) {
    currentBranch = branchTodo.top();
    branchTodo.pop();

    AABB* curLeft = currentBranch->left;
    AABB *curRight = currentBranch->right;

    // Process left node
    if (curLeft != nullptr && curLeft->pureCollision(obj)){
      if (AABBBranch *branch = dynamic_cast<AABBBranch *>(curLeft))
        branchTodo.push(branch);
      else if (curLeft->isColliding(obj))
        colliders.push_back(curLeft);
    }

    // Process right node
    if (curRight != nullptr && curRight->pureCollision(obj)){
      if (AABBBranch *branch = dynamic_cast<AABBBranch *>(curRight))
        branchTodo.push(branch);
      else if (curRight->isColliding(obj))
        colliders.push_back(curRight);
    }
  }
  return colliders;
}

bool AABBBranch::isColliding(AABB &obj) {
  if(pureCollision(obj))
    return (left != nullptr && left->isColliding(obj) ) ||
      (right != nullptr && right->isColliding(obj) );
  return false;
}
