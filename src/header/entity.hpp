#pragma once

#include "aabb.hpp"
#include "game.hpp"
#include "movingaabb.hpp"
#include "model.hpp"
#include <array>

class Entity: public MovingAABB {
protected:
  Model model;


  /* the front direction of the entity,
   * described using two angles, theta and phi,
   * theta = angle from YZ plane, horizontal
   * phi = angle from the XZ plane, vertical
   * Refer to polar coordinates in 3D for more information
   */
  std::array<float, 2> direction;

public:
  Entity(Model model, Vector3 iPos, std::array<float, 2> iDir);

  /**
   * @brief      Changes the direction of the entity.
   *
   * @details    Adds the given angles to the state angels to rotate the entity
   *
   * @param      theta: float = Angle to add
   *
   * @param      phi: float = Angle to add
   *
   * @return     return void
   */
  virtual void changeDirection(float theta, float phi) {
    direction[0] += theta;
    direction[1] += phi;
  }

  virtual void render();
};
