#include "header/entity.hpp"
#include "header/game.hpp"
#include "header/model.hpp"
#include "header/movingaabb.hpp"
#include <GL/gl.h>
#include <array>

Entity::Entity(Model model, Vector3 iPos, std::array<float, 2> iDir)
  : model(model), direction(iDir) , MovingAABB{model.getMin(), model.getMax(), iPos}{}

void Entity::render() {

  if (color == RED) { // get colour from global state
    glPushMatrix();
    {
      glTranslatef(pos[0], pos[1], pos[2]);
      glRotatef(direction[0], 1.0, 0, 0);     // vertical angle
      glRotatef(direction[1], 0.0, 1.0, 0.0); // horizontal angle
      model.draw();
    }
    glPopMatrix();
  }
}
