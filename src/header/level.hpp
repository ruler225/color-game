#pragma once

#include "deadly_platform.hpp"
#include "platform.hpp"
#include "passive_entity.hpp"
#include "event_collider.hpp"
#include "light.hpp"
#include <vector>

using std::vector;

class Level {
private:
  Vector3 playerInitPos;
  vector<Platform> platforms;
  vector<DeadlyPlatform> deadlyPlatforms;
  vector<PassiveEntity> entities;
  vector<Light> lights;
  vector<EventCollider> eventColliders;

  Color initialColor;

public:
  Level();
  Level(Vector3 pInit);

  void addPlatform(Platform plat);
  void addPlatforms(vector<Platform> platforms);
  vector<Platform> &getPlatforms();

  void addDeadlyPlatform(DeadlyPlatform deadlyPlat);
  void addDeadlyPlatforms(vector<DeadlyPlatform> deadlyPlatforms);
  vector<DeadlyPlatform> &getDeadlyPlatforms();

  void addEntity(PassiveEntity entity);
  void addEntities(vector<PassiveEntity> entities);
  vector<PassiveEntity> &getEntities();

  void addLight(Light light);
  void addLights(vector<Light> lights);
  vector<Light> &getLights();

  void addEventCollider(EventCollider);
  void addEventColliders(vector<EventCollider>);
  vector<EventCollider> &getEventColliders();

  Vector3 getPlayerPos();

  void setInitColor(Color);
  Color getInitialColor();
};
