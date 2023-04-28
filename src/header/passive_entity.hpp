#pragma once

#include "entity.hpp"
#include "model.hpp"
class PassiveEntity: public Entity {
 private:

 public:
  PassiveEntity(Model model, float *const iPos, float *const iDir);

};
