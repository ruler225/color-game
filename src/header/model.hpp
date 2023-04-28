#pragma once

#include <utility>
#include <vector>
#include <array>
#include "game.hpp"

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
  vector<Texture> textures;
  vector<Face> faces;
  const char *texPath; // ?

  Vector3 min;
  Vector3 max;

  void importObj(const char *objPath);

  void minV(Vector3 a, Vector3 b, Vector3 c);
  void maxV(Vector3 a, Vector3 b, Vector3 c);

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
  Model(const char *const objPath,
        const char *const texPath); // what is best way to pass strings

  /**
   * @brief      Draws the 3D model using OpenGL
   *
   * @details    A loop that draws the faces of the model using GL_TRIANGLES in
   * OpenGL.
   *
   * @return     void
   */
  void draw(); // calls the openGL functions to draw object.

  Vector3 getMin() { return min; }
  Vector3 getMax() { return max; }

  Model& operator=(const Model &other);
};
