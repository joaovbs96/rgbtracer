#pragma once

#include <math.h>
#include <stdlib.h>
#include "math.hpp"

class float3 {
 public:
  float3() : x(0.0f), y(0.0f), z(0.0f) {}
  float3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
  float3(float xyz_) : x(xyz_), y(xyz_), z(xyz_) {}

  inline const float3& operator+() const { return *this; }
  inline float3 operator-() const { return float3(-x, -y, -z); }

  inline float3& operator+=(const float3& v);
  inline float3& operator-=(const float3& v);
  inline float3& operator*=(const float3& v);
  inline float3& operator/=(const float3& v);
  inline float3& operator*=(const float t);
  inline float3& operator/=(const float t);

  float x, y, z;
};

inline float3& float3::operator+=(const float3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

inline float3& float3::operator-=(const float3& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

inline float3& float3::operator*=(const float3& v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

inline float3& float3::operator/=(const float3& v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

inline float3& float3::operator*=(const float t) {
  x *= t;
  y *= t;
  z *= t;
  return *this;
}

inline float3& float3::operator/=(const float t) {
  float k = 1.0f / t;
  x *= k;
  y *= k;
  z *= k;
  return *this;
}

inline float3 operator+(const float3& v0, const float3& v1) {
  return float3(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z);
}

inline float3 operator-(const float3& v0, const float3& v1) {
  return float3(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z);
}

inline float3 operator*(const float3& v0, const float3& v1) {
  return float3(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z);
}

inline float3 operator/(const float3& v0, const float3& v1) {
  return float3(v0.x / v1.x, v0.y / v1.y, v0.z / v1.z);
}

inline float3 operator*(const float3& v, const float t) {
  return float3(v.x * t, v.y * t, v.z * t);
}

inline float3 operator*(const float t, const float3& v) {
  return float3(v.x * t, v.y * t, v.z * t);
}

inline float3 operator/(const float3& v, const float t) {
  return float3(v.x / t, v.y / t, v.z / t);
}

inline float dot(const float3& v0, const float3& v1) {
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

inline float3 cross(const float3& v0, const float3& v1) {
  float x = v0.y * v1.z - v0.z * v1.y;
  float y = -v0.x * v1.z + v0.z * v1.x;
  float z = v0.x * v1.y - v0.y * v1.x;
  return float3(x, y, z);
}

inline float3 sqrt(const float3& v) {
  return float3(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z));
}

inline float length(const float3& v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
inline float squared_length(const float3& v) {
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline float3 normalize(const float3& v) {
  float k = 1.0f / length(v);
  return v * k;
}

// return pairwise min vector
float3 min_float3(float3 a, float3 b) {
  return float3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
}

// return pairwise max vector
float3 max_float3(float3 a, float3 b) {
  return float3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
}

// return max component of vector
float max_component(float3 a) { return fmaxf(fmaxf(a.x, a.y), a.z); }

// return max component of vector
float min_component(float3 a) { return fminf(fminf(a.x, a.y), a.z); }

// clamp vector
float3 clamp(float3 value, float min, float max) {
  return float3(clamp(value.x, min, max),   // x
                clamp(value.y, min, max),   // y
                clamp(value.z, min, max));  // z
}

bool isNull(const float3& v) {
  return ((v.x == 0.0f) && (v.y == 0.0f) && (v.z == 0.0f)) ? true : false;
}

// replace NaN components with zero
float3 de_nan(const float3& c) {
  float3 temp = c;

  if (temp.x != temp.x) temp.x = 0.0f;
  if (temp.y != temp.y) temp.y = 0.0f;
  if (temp.z != temp.z) temp.z = 0.0f;

  return temp;
}
