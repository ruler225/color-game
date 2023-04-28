#include "header/player.hpp"
#include "header/camera.hpp"
#include "maths.cpp"
#include "header/gameController.hpp"
#include "header/physicscontroller.hpp"
#include <limits>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

Player::Player(Model model, Vector3 iPos, std::array<float, 2> iDir)
  : Entity(model, iPos, iDir, RED) {
  movingLeft = movingRight = movingForward = movingDown = jumping = false;
}

Player::Player(Model model) : Entity(model, {0, 0, 0}, {0, 0}, RED) {}

void Player::changeColor(Color color) {
  this->color = color; // is this needed?
  GameController::getInstance().setColour(color);
}

void Player::jump() {
  if (isOnFloor()){
    this->velocity[1] += PLAYER_JUMP_VELOCITY;
    jumping = true;
  }
}

void Player::input(unsigned char key, int x, int y) {
  switch (key) {
  case ' ':
    jump();
    break;
  case 'w':
  case 'W':
    movingForward = true;
    movingDown = false;
    break;
  case 's':
  case 'S':
    movingDown = true;
    movingForward = false;
    break;
  case 'a':
  case 'A':
    movingLeft = true;
    movingRight = false;
    break;
  case 'd':
  case 'D':
    movingRight = true;
    movingLeft = false;
    break;
  }
}

void Player::keyUp(unsigned char key, int x, int y) {
  switch (key) {
  case ' ':
    jumping = false;
    break;
  case 'w':
  case 'W':
    movingForward = false;
    break;
  case 's':
  case 'S':
    movingDown = false;
    break;
  case 'a':
  case 'A':
    movingLeft = false;
    break;
  case 'd':
  case 'D':
    movingRight = false;
    break;
  default:
    break;
  }
}

void Player::setPosition(Vector3 pos) { this->pos = pos; }

void Player::rotate(std::array<float, 2> cDir) {
  direction[0] += cDir[0];
  direction[1] += cDir[1];
}

void Player::update(int timeDelta) {

  float accFactor = 1;
  if (!isOnFloor())
    accFactor = 0.2;
  Vector3 forward = getForwardAcc(direction[0], direction[1], PLAYER_ACCELERATION, timeDelta * accFactor);
  Vector3 right =
      getRightAcc(direction[0], direction[1], PLAYER_ACCELERATION, timeDelta * accFactor);

  const Vector3 proj_vel_forward = project_onto(velocity, forward);
  const Vector3 proj_vel_right = project_onto(velocity, right);

  //////////////////// Handle forward and backward movement ////////////////////
  if (!(movingForward || movingDown)) {
    ////////////////////  Not moving forward nor backwards  ////////////////////
    // decelerate forward directing
    if (magnitude(proj_vel_forward) > std::numeric_limits<float>::epsilon())
      velocity =
          subtract(velocity, scalar_multiply(proj_vel_forward, 0.8 * accFactor));
    else
      velocity = subtract(velocity, proj_vel_forward);
  } else {
    //////////////////// Moving either Forward or backwards ////////////////////
    float direction_dot = dot(velocity, forward);
    if (movingForward) {     // Moving forward
      if (direction_dot < 0) // was going backwards
        velocity =
          subtract(velocity, scalar_multiply(proj_vel_forward, 0.8 * accFactor));

      velocity = add(velocity, forward);
    } else {                 // Moving backwards
      if (direction_dot > 0) // was going forward
        velocity = subtract(velocity, scalar_multiply(proj_vel_forward, 0.8 * accFactor));
      velocity = subtract(velocity, forward);
    }
  }

  //////////////////////  Handle left and right movement  //////////////////////
  if (! (movingRight || movingLeft)) { // Not moving left nor right
    // Decelerate sideways movements
    if (magnitude(proj_vel_right) > std::numeric_limits<float>::epsilon())
      velocity = subtract(velocity, scalar_multiply(proj_vel_right, 0.8 * accFactor));
    else
      velocity = subtract(velocity, proj_vel_right);
  } else {
    float direction_dot = dot(velocity, right);
    if (movingRight) { // Moving right
      if (direction_dot < 0) // was going left
        velocity =
            subtract(velocity, scalar_multiply(proj_vel_right, 0.8 * accFactor));
      velocity = add(velocity, right);
    } else { // Moving left
      if (direction_dot > 0) // was going right
        velocity =
            subtract(velocity, scalar_multiply(proj_vel_right, 0.8 * accFactor));
      velocity = subtract(velocity, right);
    }
  }

  //////////////////////////////  Handle jumping  //////////////////////////////
  if (!jumping)
    accelerate(0, -3 * GRAVITY_CONST * timeDelta / 1000, 0);

  //////////////////////////  The usual update stuff  //////////////////////////
  pureUpdate(timeDelta);
}

void Player::render(Color gameColour) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // gluLookAt(25, 10, 25, 0, 0, 0, 0, 1, 0); // Only for testing
  Vector3 camPos = add(pos, CAMERA_POS_OFFSET);
  camPos = add(
      camPos,
      scalar_multiply(getForwardAcc(direction[0], direction[1], 1, 1000), -3));
  Vector3 camUp = getCameraUp(direction[0], direction[1]);
  Vector3 camForward = getCameraForward(direction[0], direction[1]);
  camForward = add(camPos, camForward);

  gluLookAt(camPos[0], camPos[1], camPos[2], camForward[0], camForward[1],
            camForward[2], camUp[0], camUp[1], camUp[2]);

  glPushMatrix();
  {
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef((direction[0] / M_PI) * 180, 0, 1.0, 0);     // horizontal angle
    model.draw();
  }
  glPopMatrix();
}

bool Player::isOnFloor(){
  AABB floorChecker{{min[0], 0, min[2]},
                    {max[0], 0.05, max[2]},
                    {pos[0], pos[1] - 0.05f, pos[2]}};

  return PhysicsController::getInstance().isColliding(floorChecker);
}
