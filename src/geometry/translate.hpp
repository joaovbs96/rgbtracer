#pragma once

#include "geometry.hpp"

class Translate : public Geometry {
 public:
  Translate() {}
  Translate(Geometry* obj, const float3& p) : obj(obj), p(p) {}

  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const override {
    Ray moved_r(r.origin() - p, r.direction(), r.time());
    if (!obj->hit(moved_r, hit_rec, t_min, t_max)) return false;

    hit_rec.p += p;
    // face normals?

    return true;
  }

  virtual bool bounding_box(float t0, float t1, AABB& output) const override {
    if (!obj->bounding_box(t0, t1, output)) return false;

    output = AABB(output.min() + p, output.max() + p);

    return true;
  }

  virtual float3 center() const override { return obj->center() + p; }

  Brdf* getBrdf() const override { return obj->getBrdf(); }

  float3 p;
  Geometry* obj;
};