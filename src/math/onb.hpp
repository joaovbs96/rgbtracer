#pragma once

#include "float3.hpp"

class ONB {
 public:
  ONB(const float3& n) {
    w = normalize(n);
    float3 a = (fabsf(w.x) > 0.9f) ? float3(0.0f, 1.0f, 0.0f)
                                   : float3(1.0f, 0.0f, 0.0f);

    v = normalize(cross(w, a));
    u = cross(w, v);
  }

  float3 local(float a, float b, float c) const {
    return a * u + b * v + c * w;
  }

  float3 local(const float3& a) const { return a.x * u + a.y * v + a.z * w; }

  float3 u, v, w;
};