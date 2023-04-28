#pragma once

#include "game.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class AABB {
protected:
  Vector3 min;
  Vector3 max;
  Color color;
  /* The current position of the entity
     Used to move the model using glTranslate
  */
  Vector3 pos;
  // Checks for collision using AABB method
  bool pureCollision(AABB&);

  friend class AABBBranch;

public:
  // Constructors
  AABB(Vector3 minVec, Vector3 maxVec, Vector3 pos, Color c = RED);
  AABB();

  ////////////////////////////// Member functions //////////////////////////////
  float area();
  // Same as pureCollision but checks if colour matches game state
  virtual bool isColliding(AABB &obj);
  Color getColor();

  // Getter functions for min and max
  Vector3 getMin();
  Vector3 getMax();
};
