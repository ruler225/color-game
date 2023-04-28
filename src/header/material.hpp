#pragma once

#include "light.hpp"
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

struct Material {
  const Vector4 amb;
  const Vector4 dif;
  const Vector4 spec;
  const float shin;

  void set() const {
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb.data());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif.data());
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec.data());
    glMaterialf(GL_FRONT, GL_SHININESS, shin);
  }
};

const Material redMat{{0.8, 0, 0, 1}, {0.8, 0, 0, 1}, {0.4, 0.1, 0.1, 1}, 0.9};
const Material greenMat{{0, 0.8, 0, 1}, {0, 0.8, 0, 1}, {0.1, 0.4, 0.1, 1}, 0.9};
const Material blueMat{{0, 0, 0.8, 1}, {0, 0, 0.8, 1}, {0.1, 0.1, 0.4, 1}, 0.9};
