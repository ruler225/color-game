#pragma once

#include "game.hpp"
#include "platform.hpp"
#include "passive_entity.hpp"
#include <vector>
using std::vector;

class Level {
private:
  const Vector3 playerInitPos;
  vector<Platform> platforms;
  vector<PassiveEntity> entities;
public:
  Level(Vector3 pInit): playerInitPos(pInit){}

  void addPlatform(Platform plat) {platforms.push_back(plat);}
  void addPlatforms(vector<Platform> platforms) {
    for (size_t i = 0; i < platforms.size(); i++) {
      this->platforms.push_back(platforms[i]);
    }
  }

  vector<Platform> getPlatforms() {return platforms;}
  vector<PassiveEntity> getEntities() { return entities; }

  void addEntity(PassiveEntity entity) {entities.push_back(entity);}
  void addEntitys(vector<PassiveEntity> entities) {
    for (size_t i = 0; i < entities.size(); i++)
      this->entities.push_back(entities[i]);
  }

  Vector3 const getPlayerPos() { return playerInitPos; }
};
