#include "header/camera.hpp"

Vector3 getCameraForward(float theta, float phi) {
  Vector3 forward;
  forward[0] = sin(theta) * cos(phi);
  forward[1] = sin(phi);
  forward[2] = cos(theta) * cos(phi);
  return forward;
}

Vector3 getCameraUp(float theta, float phi) {
  Vector3 up;
  up[0] = sin(theta) * cos(phi + M_PI / 2);
  up[1] = sin(phi + M_PI / 2);
  up[2] = cos(theta) * cos(phi + M_PI / 2);
  return up;
}
