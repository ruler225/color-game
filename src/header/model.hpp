#pragma once

#include <string>
#include <vector>
#include <array>
#include "game.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using std::vector;
using std::array;

using Texture = array<float, 2>;

struct Face {
  // references to the vectors
  int verts[3];
  int norms[3];
  int textures[3];

  // might add more parameters for materials
};

/**
 * @brief      A class to store and handle models
 *
 * @details    A class to handle importing, storing, and drawing models. It imports them from an .obj file and stores the data in vectors.
 */
class Model {
private:
  vector<Vector3> vertices;
  vector<Vector3> normals;
  vector<Texture> textureCoordinates;
  vector<Face> faces;

  GLuint textureBinding;
  unsigned int tWidth;
  unsigned int tHeight;
  bool textured;

  Vector3 min;
  Vector3 max;

  void importObj(const std::string &objPath);
  void loadTexture(const std::string &texPath);

  Vector3 minV(Vector3 a, Vector3 b);
  Vector3 maxV(Vector3 a, Vector3 b);

  void computeMinMax();

public:
  /**
   * @brief      Constructs a new model object
   *
   * @details    Imports and constructs a new model object from an .obj file and
   * an optional texture file.
   *
   * @param      objPath: char* = The path to the object file.
   *
   * @param      texPath: char* = The path to the texture file.
   *
   * @return     A new Model object.
   */
  Model(const std::string &objPath,
        const std::string &texPath); // what is best way to pass strings

  /**
   * @brief      Draws the 3D model using OpenGL
   *
   * @details    A loop that draws the faces of the model using GL_TRIANGLES in
   * OpenGL.
   *
   * @return     void
   */
  void draw(); // calls the openGL functions to draw object.

  Vector3 getMin();
  Vector3 getMax();
};
