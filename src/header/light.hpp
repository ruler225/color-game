#pragma once

#include "game.hpp"
#include <array>
#include <fstream>
#include <string>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

using Vector4 = std::array<float, 4>;

struct Light {
  Vector4 pos;
  Vector4 amb;
  Vector4 dif;
  Vector4 spec;

  GLenum light;

  void render() {
    glEnable(light);
    glLightfv(light, GL_POSITION, pos.data());
    glLightfv(light, GL_AMBIENT, amb.data());
    glLightfv(light, GL_SPECULAR, spec.data());
    glLightfv(light, GL_DIFFUSE, dif.data());
  }
};
