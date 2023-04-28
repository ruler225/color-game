#pragma once

#include "aabb.hpp"
#include "model.hpp"
#include "platform.hpp"
class DeadlyPlatform: Platform {
public:
  DeadlyPlatform(Model model, Vector3 iPos, Color c): Platform(model, iPos, c) {}
  bool isColliding(AABB);
};
