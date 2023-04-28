#include "header/game.hpp"
#include <cmath>

float constexpr dot(const Vector3 &a, const Vector3 &b) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float constexpr magnitude(const Vector3 &v) {
  return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

Vector3 constexpr add(const Vector3 &a, const Vector3 &b) {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}

Vector3 constexpr subtract(const Vector3 &a, const Vector3 &b) {
  return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

Vector3 constexpr scalar_multiply(const Vector3 &v, const float s) {
  return {v[0] * s, v[1] * s, v[2] * s};
}

Vector3 constexpr normalised(const Vector3 &v) {
  float mag = magnitude(v);
  return {v[0] / mag, v[1] / mag, v[2] / mag};
}

Vector3 constexpr project_onto(const Vector3 &a, const Vector3 &b) {
  Vector3 bNorm = normalised(b);
  float dot1 = dot(a, b);
  return scalar_multiply(bNorm, dot1);
}
