#pragma once

#include "aabb.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "game.hpp"

// Player-specific constants
const float PLAYER_MAX_SPEED = 5;
const float PLAYER_ACCELERATION = 8.0;
const float PLAYER_JUMP_VELOCITY = 10.0;

class Player : public Entity {
private:
  bool movingLeft;
  bool movingRight;
  bool movingForward;
  bool movingDown;
  bool jumping;

  bool isOnFloor();

  friend class GameController;
public:
  // Constructor which will simply call its parent class constructor
  Player(Model model, Vector3 iPos, std::array<float, 2> iDir);
  Player(Model model);

  void setPosition(Vector3 pos);
  void rotate(std::array<float, 2> iDir);
  // Change player colour to specified colour
  void changeColor(Color color);

  // Checks if player is currently on the ground, and sets upward velocity to a
  // predetermined amount (TBD)
  void jump();

  // Handle input commands for the player.
  void input(unsigned char key, int x, int y);
  void keyUp(unsigned char key, int x, int y);
  void update(int timedelta) override;
  void render(Color) override;
};
