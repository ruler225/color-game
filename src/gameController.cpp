#include "header/gameController.hpp"
#include "header/game.hpp"
#include "header/model.hpp"
#include "header/player.hpp"
#include <array>
#include <cstddef>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif


////////////////////////////  Game Controller data  ////////////////////////////

GameController::GameController() : p{Model{"models/player.obj", "textures/player.png"}}, lives(3) {}

GameController &GameController::getInstance() {
  static GameController gc;
  return gc;
}

void GameController::startLevel(Level l) {
  currentLevel = l;
  p.setPosition(l.getPlayerPos());
  gameColor = l.getInitialColor();

  playerCheckpointPos = l.getPlayerPos();
  checkpointColor = gameColor;
}

Color GameController::getColor() { return gameColor; }
void GameController::setColour(Color newColor) { gameColor = newColor; }

bool GameController::getGameState() { return gameRunning; }
void GameController::setGameState(bool newState) { gameRunning = newState; }

void GameController::killPlayer() {
  lives--;
  if (lives <= 0){
    std::cout << "Game over" << std::endl;
    exit(0);
  }
  loadCheckpoint();
}

void GameController::endGame() {
  std::cout << "Game finished!" << std::endl;
  exit(0);
}

void GameController::saveCheckpoint() {
  playerCheckpointPos = p.pos;
  checkpointColor = gameColor;
}

void GameController::loadCheckpoint() {
  p.setPosition(playerCheckpointPos);
  p.velocity = {0,0,0};
  gameColor = checkpointColor;
}
