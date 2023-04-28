#include "header/model.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <FreeImage.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using std::basic_ifstream;
using std::getline;
using std::invalid_argument;
using std::istringstream;
using std::stoi;
using std::string;

Model::Model(const string &objPath, const string &texPath) {
  importObj(objPath);
  if (!texPath.empty())
    loadTexture(texPath);
  else
    textured = false;
  computeMinMax();
}

void Model::importObj(const string &objPath) {

  string line;
  string lType;
  basic_ifstream<char> file(objPath);
  if (file.is_open()) {
    while (getline(file, line)) {
      istringstream iss(line);
      if (line == "")
        continue;
      iss >> lType;
      if (lType == "v") {
        float x = 0, y = 0, z = 0;
        iss >> x >> y >> z;
        Vector3 v = {x, y, z};
        this->vertices.push_back(v);
      }
      if (lType == "vn") {
        float x = 0, y = 0, z = 0;
        iss >> x >> y >> z;
        Vector3 v = {x, y, z};
        this->normals.push_back(v);
      }
      if (lType == "vt") {
        float x = 0, y = 0;
        iss >> x >> y;
        Texture v = {x, y};
        this->textureCoordinates.push_back(v);
      }
      // need implementation
      if (lType == "f") {
        string refS;
        Face f;
        for (int i = 0; iss >> refS; i++) {
          istringstream refStream(refS);
          string refV, refN, refT;
          getline(refStream, refV, '/');
          getline(refStream, refT, '/');
          getline(refStream, refN, '/');

          f.verts[i] = stoi(refV);
          try {
            f.norms[i] = stoi(refN);
          } catch (const invalid_argument &) {
            f.norms[i] = -1;
          }
          try {
            f.textures[i] = stoi(refT);
          } catch (const invalid_argument &) {
            f.textures[i] = -1;
          }
        }
        this->faces.push_back(f);
      }
    }
    file.close();
  }
}

void Model::loadTexture(const std::string &filename) {
  glEnable(GL_TEXTURE_2D);

  FIBITMAP *bitMap =
      FreeImage_Load(FreeImage_GetFileType(filename.c_str()), filename.c_str());

  {
    FIBITMAP *temp = bitMap;
    bitMap = FreeImage_ConvertTo32Bits(bitMap);
    FreeImage_Unload(temp);
  }
  tWidth = FreeImage_GetWidth(bitMap);
  tHeight = FreeImage_GetHeight(bitMap);

  unsigned int texSize = tWidth * tHeight * 4;
  GLubyte *texMap = new GLubyte[texSize];
  {
    GLubyte *temp = FreeImage_GetBits(bitMap);
    std::copy(temp, temp + texSize, texMap);
  }

  glGenTextures(1, &textureBinding);
  glBindTexture(GL_TEXTURE_2D, textureBinding);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, (GLvoid *)texMap);

  // Texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  FreeImage_Unload(bitMap);

  textured = true;
}

void Model::draw() {
  // Load the texture
  if (textured){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBinding);
  } else {
    glDisable(GL_TEXTURE_2D);
  }

  //////////////////////////////  Draw the model  //////////////////////////////
  // Draw the faces
  for (Face f : faces) {
    glBegin(GL_TRIANGLES);
    {
      // draw each vertex
      for (int i = 0; i < 3; i++) {
        // try setting normal
        if (!(f.norms[i] < 0))
          glNormal3fv(normals[f.norms[i]-1].data());
        // try setting texture mapping
        if (!(f.textures[i] < 0))
          glTexCoord2fv(textureCoordinates[f.textures[i]-1].data());
        // draw vertex
        glVertex3fv(vertices[f.verts[i]-1].data());
      }
    }
    glEnd();
  }
}

Vector3 Model::getMin() { return min; }

Vector3 Model::getMax() { return max; }

void Model::computeMinMax() {
  min = vertices[0];
  max = vertices[0];
  for (size_t i = 1; i < vertices.size(); i++) {
    min = minV(min, vertices[i]);
    max = maxV(max, vertices[i]);
  }
}

Vector3 Model::minV(Vector3 a, Vector3 b) {
  Vector3 c;
  c[0] = std::min(a[0], b[0]);
  c[1] = std::min(a[1], b[1]);
  c[2] = std::min(a[2], b[2]);
  return c;
};

Vector3 Model::maxV(Vector3 a, Vector3 b) {
  Vector3 c;
  c[0] = std::max(a[0], b[0]);
  c[1] = std::max(a[1], b[1]);
  c[2] = std::max(a[2], b[2]);
  return c;
};
