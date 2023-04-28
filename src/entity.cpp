#include "header/entity.hpp"
#include "header/game.hpp"
#include "header/model.hpp"
#include "header/movingaabb.hpp"
#include <array>
#include <cmath>

#ifdef __APPLE__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

Entity::Entity(Model model, Vector3 iPos, std::array<float, 2> iDir, Color c)
  : MovingAABB{model.getMin(), model.getMax(), iPos, c}, model(model),
      direction(iDir) {}

void Entity::render(Color gameColor) {

  if (color == gameColor) { // get colour from global state
    glPushMatrix();
    {
      glTranslatef(pos[0], pos[1], pos[2]);
      glRotatef((direction[0] / M_PI) * 180, 1.0, 0, 0);     // vertical angle
      glRotatef((direction[1] / M_PI) * 180, 0.0, 1.0, 0.0); // horizontal angle
      model.draw();
    }
    glPopMatrix();
  }
}
void Entity::changeDirection(float theta, float phi) {
  direction[0] += theta;
  direction[1] += phi;
  // if (direction[1] > M_PI / 2)
  //   direction[1] = M_PI / 2;
  // if (direction[1] < -M_PI / 2)
  //   direction[1] = -M_PI / 2;
}
