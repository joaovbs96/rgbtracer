#pragma once

#include "../utils/aabb.hpp"
#include "../utils/prd.hpp"
#include "../utils/ray.hpp"

class Geometry {
 public:
  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const = 0;

  virtual bool bounding_box(float t0, float t1, AABB& output) const = 0;

  virtual float3 center() const = 0;

  virtual Brdf* getBrdf() const { return nullptr; }

  virtual float3 sample(const float3& p, const float3& wo, const float3& n,
                        uint& seed) const {
    return float3(0.0f);
  }

  virtual float pdf(const float3& p, const float3& wo, const float3& wi,
                    const float3& n) const {
    return 0.0f;
  }
};

float2 sphere_uv(const float3& p) {
  float phi = atan2f(p.z, p.x);
  float theta = asinf(p.y);

  return float2(1.0f - (phi + PI) / TWO_PI, (theta + PI / 2.0f) / PI);
}
