#include "header/platform.hpp"
#include "header/aabb.hpp"
#include "header/model.hpp"
#include <GL/gl.h>

Platform::Platform(Model model, Vector3 iPos, Color c)
    : model(model), AABB(model.getMin(), model.getMax(), iPos, c) {}

void Platform::render(){
  if (getColor() == RED){ // get colour from global state
    glPushMatrix();
    {
      glTranslatef(pos[0], pos[1], pos[2]);
      model.draw();
    }
    glPopMatrix();
  }
}
