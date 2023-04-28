#include "header/deadly_platform.hpp"
#include "header/gameController.hpp"
#include "header/player.hpp"
#include <typeinfo>

DeadlyPlatform::DeadlyPlatform(Model model, Vector3 iPos, Color c)
    : Platform(model, iPos, c) {}

bool DeadlyPlatform::isColliding(AABB &obj) {
  if (AABB::isColliding(obj)){
    if (typeid(obj).hash_code() == typeid(Player).hash_code()){
      GameController::getInstance().killPlayer();
      return false;
    }
    return true;
  }
  return false;
}
