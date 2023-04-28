#pragma once

#include "aabb.hpp"
class EventCollider : public AABB {
private:
  void (*eventFunction)();

public:
  EventCollider(void (*)(), AABB);

  bool isColliding(AABB &obj) override;
};
