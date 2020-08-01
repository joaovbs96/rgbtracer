#pragma once

#include "geometry.hpp"

class Flip : public Geometry {
 public:
  Flip(Geometry* obj) : obj(obj) {}

  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const override {
    if (!obj->hit(r, hit_rec, t_min, t_max)) return false;

    hit_rec.n *= -1;
    return true;
  }

  virtual bool bounding_box(float t0, float t1, AABB& output) const override {
    return obj->bounding_box(t0, t1, output);
  }

  virtual float3 center() const override { return obj->center(); }

  Brdf* getBrdf() const override { return obj->getBrdf(); }

  float3 sample(const float3& p,   // next ray origin
                const float3& wo,  // previous ray direction
                const float3& n,   // geometric normal
                uint& seed) const override {
    return obj->sample(p, wo, n, seed);
  }

  float pdf(const float3& p,   // origin of next ray
            const float3& wo,  // direction of current ray
            const float3& wi,  // direction of next ray
            const float3& n)   // geometric normal
      const override {
    return obj->pdf(p, wo, wi, n);
  }

  Geometry* obj;
};