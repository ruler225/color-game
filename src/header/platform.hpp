#pragma once

#include "model.hpp"
#include "aabb.hpp"

class Platform: public AABB { // inherits from AABB
protected:
  Model model;
public:
  Platform(Model model, Vector3 iPos, Color c);
  void render();
};
