#include "header/model.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
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

using std::array;
using std::basic_ifstream;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::stoi;
using std::string;
using std::out_of_range;

Model::Model(const char *const objPath, const char *const texPath)
    : texPath(texPath) {
  this->importObj(objPath);
  computeMinMax();
}

void Model::importObj(const char *objPath) {

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
        this->textures.push_back(v);
      }
      // need implementation
      if (lType == "f") {
        string refS;
        Face f;
        for (int i = 0; iss >> refS; i++) {
          istringstream refStream(refS);
          string refV, refN, refT;
          getline(refStream, refV, '/');
          getline(refStream, refN, '/');
          getline(refStream, refT, '/');

          f.verts[i] = stoi(refV);
          try {
            f.norms[i] = stoi(refN);
          } catch (const std::invalid_argument) {
            f.norms[i] = -1;
          }
          try {
            f.textures[i] = stoi(refT);
          } catch (const std::invalid_argument) {
            f.textures[i] = -1;
          }
        }
        this->faces.push_back(f);
      }
    }
    file.close();
  }
}

void Model::draw() {
  // Consider storing the material
  for (size_t i = 0; i < faces.size(); i++) {
    Face f = faces[i];
    glBegin(GL_TRIANGLES);
    {
      for (int i = 0; i < 3; i++) {
        // try setting normal
        try {
          glNormal3fv(normals[f.norms[i]].data());
        } catch (out_of_range) {
        }
        // try setting texture mapping
        try {
          glTexCoord2fv(textures[f.textures[i]].data());
        } catch (out_of_range) {
        }
        glVertex3fv(normals[f.verts[i]].data());
      }
    }
    glEnd();
  }
}

Model& Model::operator=(const Model &other) {
  if(this != &other) {
    delete this->texPath;
    this->texPath = other.texPath;
    this->vertices = other.vertices;
    this->normals = other.normals;
    this->textures = other.textures;
    this->faces = other.faces;
    this->min = other.min;
    this->max = other.max;
  }

  return *this;
}

Vector3 Model::getMin() { return min; }

Vector3 Model::getMax() { return max; }

void Model::computeMinMax(){
  min = vertices[0];
  max = vertices[0];
  for (size_t i = 1; i < vertices.size(); i++) {
    minV(min, vertices[i], min);
    maxV(max, vertices[i], min);
  }
}

void Model::minV(Vector3 a, Vector3 b, Vector3 c) {
  c[0] = std::min(a[0], b[0]);
  c[1] = std::min(a[1], b[1]);
  c[2] = std::min(a[2], b[2]);
};

void Model::maxV(Vector3 a, Vector3 b, Vector3 c) {
  c[0] = std::max(a[0], b[0]);
  c[1] = std::max(a[1], b[1]);
  c[2] = std::max(a[2], b[2]);
};
