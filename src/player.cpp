#include "header/player.hpp"
#include "header/physicscontroller.hpp"

Player::Player(Model model, Vector3 iPos, std::array<float, 2> iDir): Entity::Entity(model, iPos, iDir) {
    movingLeft, movingRight, movingUp, movingDown = false;
    Vector3 floorMin = {min[0], min[1] - 0.1, min[2]};
    floorAABB = AABB(floorMin, max, pos);
}

void Player::changeColor(Color color) {
    this->color = color;
}

void Player::jump() {
    this->velocity[1] += PLAYER_JUMP_VELOCITY;
}

void Player::input(unsigned char key, int x, int y) {
    PhysicsController *physics = PhysicsController::getInstance();
    switch (key) {
        case ' ':
            if (physics->hasCollider(floorAABB))
                this->jump();
            break;
        //TODO: Determine player's direction relative to camera. For now, the player's position will be changed absolutely (as opposed to relative to a camera)
        case 'w':
        case 'W':
            movingUp = true;
            break;
        case 's':
        case 'S':
            movingDown = true;
            break;
        case 'a':
        case 'A':
            movingLeft = true;
            break;
        case 'd':
        case 'D':
            movingRight = true;
            break;
        //TODO: this is probably a hacky solution, this should be changed
        default:
            movingUp, movingDown, movingLeft, movingRight = false;
    }
}


void Player::update(int timedelta) {
    //Again, no relative positions are implemented yet depending on player direction
    if (movingUp && this->velocity[2] < PLAYER_MAX_SPEED) {
        this->accelerate(0, 0, (timedelta*PLAYER_ACCELERATION)/1000.0);
    }
    if (movingDown && this->velocity[2] > -PLAYER_MAX_SPEED) {
        this->accelerate(0, 0, -(timedelta*PLAYER_ACCELERATION)/1000.0);
    }
    if (movingLeft && this->velocity[0] > -PLAYER_MAX_SPEED) {
        this->accelerate(-(timedelta*PLAYER_ACCELERATION)/1000.0, 0, 0);
    }
    if (movingRight && this->velocity[0] < PLAYER_MAX_SPEED) {
        this->accelerate((timedelta*PLAYER_ACCELERATION)/1000.0, 0, 0);
    }
    //Decelerate where there is no key being held
    if (!movingUp && this->velocity[2] > 0) {
        if (this->velocity[2] < 0.1)
            this->velocity[2] = 0;
        else
            this->accelerate(0, 0, -(timedelta*PLAYER_ACCELERATION)/1000.0);
    }
    if (!movingDown && this->velocity[2] < 0) {
        if (this->velocity[2] > -0.1)
            this->velocity[2] = 0;
        else
            this->accelerate(0, 0, (timedelta*PLAYER_ACCELERATION)/1000.0);
    }
    if (!movingLeft && this->velocity[0] < 0) {
        if (this->velocity[0] > -0.1)
            this->velocity[0] = 0;
        else
            this->accelerate((timedelta*PLAYER_ACCELERATION)/1000.0, 0, 0);
    }
    if (!movingRight && this->velocity[0] > 0) {
        if (this->velocity[0] < 0.1)
            this->velocity[0] = 0;
        else
            this->accelerate(-(timedelta*PLAYER_ACCELERATION)/1000.0, 0, 0);
    }      
    
    //The usual update stuff
    pos[0] += velocity[0] * (timedelta / 1000.0f);
    pos[1] += velocity[1] * (timedelta / 1000.0f);
    pos[2] += velocity[2] * (timedelta / 1000.0f);
}