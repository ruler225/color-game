#pragma once

#include "game.hpp"
#include "model.hpp"
#include "aabb.hpp"

class Platform: public AABB { // inherits from AABB
protected:
  Model model;
public:
  Platform(Model model, Vector3 iPos, Color c = RED);
  void render(Color);
};
