#pragma once

#include "aabb.hpp"
#include "model.hpp"
#include "platform.hpp"
class DeadlyPlatform : public Platform {
public:
  DeadlyPlatform(Model model, Vector3 iPos, Color c = RED);
  bool isColliding(AABB &) override;
};
