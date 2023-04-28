#pragma once

#include "level.hpp"
#include "player.hpp"

class GameController {
private:
  GameController();

  // Game state variables
  Level currentLevel;
  Color gameColor;
  bool gameRunning = true;

  // Player variables
  Player p;
  int lives;

  // checkpoint data
  Vector3 playerCheckpointPos;
  Color checkpointColor;

  // Miscellaneous
  int width = 800;
  int height = 800;
public:
  static GameController& getInstance();

  void startLevel(Level l);
  void killPlayer();

  Level& getLevel() { return currentLevel; }
  Player& getPlayer() {return p;}

  int getWidth() { return width; }
  int getHeight() { return height; }

  void setWidth(int w) { width = w; }
  void setHeight(int h) { height = h; }

  Color getColor();
  void setColour(Color);

  bool getGameState();
  void setGameState(bool newState);

  void endGame();
  void saveCheckpoint();
  void loadCheckpoint();
};
