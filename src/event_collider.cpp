#include "header/event_collider.hpp"
#include "header/player.hpp"
#include <typeinfo>

EventCollider::EventCollider(void (*eventFunc)(), AABB aabb) : AABB(aabb), eventFunction(eventFunc){}

bool EventCollider::isColliding(AABB &obj) {
  if (AABB::isColliding(obj) && typeid(obj) == typeid(Player))
    eventFunction();
  return false;
}
