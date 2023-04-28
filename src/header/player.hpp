#pragma once

#include "aabb.hpp"
#include "entity.hpp"
#include "game.hpp"

//Player-specific constants
const float PLAYER_MAX_SPEED = 50;
const float PLAYER_ACCELERATION = 5.0;
const float PLAYER_JUMP_VELOCITY = 10.0;

class Player : public Entity {
    private:
    bool movingLeft;
    bool movingRight;
    bool movingUp;
    bool movingDown;

    AABB floorAABB;

    public:
    //Constructor which will simply call its parent class constructor
    Player(Model model, Vector3 iPos, std::array<float, 2> iDir);
    
    //Change player colour to specified colour
    void changeColor(Color color);

    //Checks if player is currently on the ground, and sets upward velocity to a predetermined amount (TBD)
    void jump();

    //Handle input commands for the player.
    void input(unsigned char key, int x, int y);
    void update(int timedelta) override;
};