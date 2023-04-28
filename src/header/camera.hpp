#pragma once

#include "game.hpp"
#include <math.h>

// get 'up' vector from the direction angles
Vector3 getCameraUp(float theta, float phi);

// get the 'lookAt' vector form the direction angles
Vector3 getCameraForward(float theta, float phi);

// Offset of camera location in relation to player position
constexpr Vector3 CAMERA_POS_OFFSET = {0, 2.7, 0};

// offset in look angle
constexpr float CAMERA_THETA_OFFSET = 0;
constexpr float CAMERA_PHI_OFFSET = 0;
