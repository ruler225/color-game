#include "header/level.hpp"
#include "header/light.hpp"

using std::vector;

Vector3 Level::getPlayerPos() { return playerInitPos; }

void Level::addPlatform(Platform plat) { platforms.push_back(plat); }
void Level::addPlatforms(vector<Platform> platforms) {
  for (size_t i = 0; i < platforms.size(); i++) {
    this->platforms.push_back(platforms[i]);
  }
}
vector<Platform> &Level::getPlatforms() { return platforms; }

void Level::addDeadlyPlatform(DeadlyPlatform deadlyplat) { deadlyPlatforms.push_back(deadlyplat); }
void Level::addDeadlyPlatforms(vector<DeadlyPlatform> deadlyPlatforms) {
  for (size_t i = 0; i < deadlyPlatforms.size(); i++) {
    this->deadlyPlatforms.push_back(deadlyPlatforms[i]);
  }
}
vector<DeadlyPlatform> &Level::getDeadlyPlatforms() { return deadlyPlatforms; }

void Level::addEntity(PassiveEntity entity) { entities.push_back(entity); }
void Level::addEntities(vector<PassiveEntity> entities) {
  for (size_t i = 0; i < entities.size(); i++)
    this->entities.push_back(entities[i]);
}
vector<PassiveEntity> &Level::getEntities() { return entities; }

void Level::addLight(Light newLight) { lights.push_back(newLight); }
void Level::addLights(vector<Light> newLights) {
  for(Light light : newLights)
    lights.push_back(light);
}
vector<Light> &Level::getLights() { return lights; }

void Level::addEventCollider(EventCollider eventCollider) {eventColliders.push_back(eventCollider);}
void Level::addEventColliders(vector<EventCollider> eventColliders) {
  for (size_t i = 0; i < eventColliders.size(); i++)
    this->eventColliders.push_back(eventColliders[i]);
}
vector<EventCollider> &Level::getEventColliders() { return eventColliders; }

void Level::setInitColor(Color c) { initialColor = c; }
Color Level::getInitialColor() { return initialColor; }

Level::Level(Vector3 pInit) : playerInitPos(pInit) {}

Level::Level(): playerInitPos({0, 0, 0}) {}
