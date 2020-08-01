#pragma once

#include <math.h>

/**
 * @brief Common Math Functions & Defines
 *
 */

typedef unsigned int uint;

#define PI 3.1415926f
#define TWO_PI 6.2831852f
#define FOUR_PI 12.5663704f
#define HALF_PI 1.5707963f
#define INV_PI 0.3183099f
#define INV_TWOPI 0.15915494f

// Axis type
typedef enum { X_AXIS, Y_AXIS, Z_AXIS, NO_AXIS } Axis;

// Returns squared number
inline float squared(const float& a) { return a * a; }
inline int squared(const int& a) { return a * a; }

// radians and degrees
inline float deg2rad(float deg) { return (PI / 180.f) * deg; }
inline float rad2deg(float rad) { return (180.f / PI) * rad; }

// returns x if it falls into [0, 1], 0 if x < 0, 1 if x > 1
float saturate(float x) { return fmaxf(0.f, fminf(1.f, x)); }

float clamp(float value, float min, float max) {
  return value < min ? min : (value > max ? max : value);
}

float PowerHeuristic(unsigned int numf, float fPdf, unsigned int numg,
                     float gPdf) {
  float f = numf * fPdf;
  float g = numg * gPdf;

  return (f * f) / (f * f + g * g);
}

struct float2 {
  float2() : u(0.0f), v(0.0f) {}
  float2(float u, float v) : u(u), v(v) {}
  float u, v;
};
