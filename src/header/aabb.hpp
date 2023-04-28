#pragma once

#include "game.hpp"


class AABB {
    //Private properties
  protected:
    Vector3 min;
    Vector3 max;
    Color color;
    /* The current position of the entity
         Used to move the model using glTranslate
      */
    Vector3 pos;

  public:
    //Constructors
  AABB(Vector3 minVec, Vector3 maxVec, Vector3 pos, Color c);
  AABB(Vector3 minVec, Vector3 maxVec, Vector3 pos);
    //Default constructor (min and max will be (0,0,0)  )
    AABB();
    //Member functions
    float area();
    virtual bool isColliding(AABB &obj);
    Color getColor();
    //Getter functions for min and max
    Vector3 getMin();
    Vector3 getMax();

};
