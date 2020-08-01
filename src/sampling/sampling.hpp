#pragma once

#include "../math/float3.hpp"
#include "../math/math.hpp"
#include "rng.hpp"

// https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/
float3 random_in_unit_sphere(uint& seed) {
  float theta = rnd(seed) * TWO_PI;
  float sinTheta = sinf(theta);
  float cosTheta = cosf(theta);

  float phi = acosf(2.0f * rnd(seed) - 1.0f);
  float sinPhi = sinf(phi);
  float cosPhi = cosf(phi);

  float r = cbrtf(rnd(seed));
  float x = r * sinPhi * cosTheta;
  float y = r * sinPhi * sinTheta;
  float z = r * cosPhi;

  return float3(x, y, z);
}

float3 random_in_unit_disk(uint& seed) {
  const float a = rnd(seed) * TWO_PI;

  const float3 xy = float3(sinf(a), cosf(a), 0);

  return xy * sqrtf(rnd(seed));
}

float3 random_unit_vector(uint& seed) {
  const float a = rnd(seed) * TWO_PI;
  const float z = (2.0f * rnd(seed)) - 1;
  const float r = sqrtf(1.0f - squared(z));

  return float3(r * cosf(a), r * sinf(a), z);
}

float3 sample_hemisphere(uint& seed) {
  const float r = sqrtf(rnd(seed));
  const float phi = TWO_PI * rnd(seed);
  const float x = r * cosf(phi), y = r * sinf(phi);
  const float z = sqrtf(fmaxf(0.0f, 1.0f - squared(x) - squared(y)));

  return float3(x, y, z);
}