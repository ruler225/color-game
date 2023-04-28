#pragma once

#include "aabb.hpp"
#include "game.hpp"
#include "model.hpp"
#include "movingaabb.hpp"
#include <array>

class Entity : public MovingAABB {
protected:
  Model model;

  /* the front direction of the entity,
   * described using two angles, theta and phi,
   * theta,0 = angle from YZ plane, horizontal
   * phi,1 = angle from the XZ plane, vertical
   * Refer to polar coordinates in 3D for more information
   */
  std::array<float, 2> direction;

public:
  Entity(Model model, Vector3 iPos, std::array<float, 2> iDir, Color c);

  /**
   * @brief      Changes the direction of the entity.
   *
   * @details    Adds the given angles to the state angels to rotate the entity.
   * The value for `phi` is clamped between +/- π/2
   *
   * @param      theta: float = Angle to add
   *
   * @param      phi: float = Angle to add
   *
   * @return     return void
   */
  virtual void changeDirection(float theta, float phi);

  virtual void render(Color);
};
