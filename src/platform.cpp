#include "header/platform.hpp"
#include "header/aabb.hpp"
#include "header/model.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

Platform::Platform(Model model, Vector3 iPos, Color c)
    : AABB(model.getMin(), model.getMax(), iPos, c), model(model) {}

void Platform::render(Color gameColour){
  if (color == gameColour){ // get colour from global state
    glPushMatrix();
    {
      glTranslatef(pos[0], pos[1], pos[2]);
      model.draw();
    }
    glPopMatrix();
  }
}
