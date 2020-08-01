#pragma once

#include "geometry.hpp"

// TODO: X and Z rotations, check github for reference

class YRotation : public Geometry {
 public:
  YRotation() {}
  YRotation(Geometry* obj, const float a) : obj(obj), rad(deg2rad(a)) {
    sintheta = sinf(rad);
    costheta = cosf(rad);
    hasbox = obj->bounding_box(0.0f, 1.0f, bbox);

    float3 min = float3(FLT_MAX), max = float3(-FLT_MAX);
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++) {
          float x = i * bbox.max().x + (1 - i) * bbox.min().x;
          float y = j * bbox.max().y + (1 - j) * bbox.min().y;
          float z = k * bbox.max().z + (1 - k) * bbox.min().z;

          float newx = costheta * x + sintheta * z;
          float newz = -sintheta * x + costheta * z;
          float3 tester(newx, y, newz);

          max = max_float3(max, tester), min = min_float3(min, tester);
        }

    bbox = AABB(min, max);
  }

  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const override {
    float3 r_origin(costheta * r.origin().x - sintheta * r.origin().z,
                    r.origin().y,
                    sintheta * r.origin().x + costheta * r.origin().z);
    float3 r_direction(costheta * r.direction().x - sintheta * r.direction().z,
                       r.direction().y,
                       sintheta * r.direction().x + costheta * r.direction().z);

    Ray rotated_r(r_origin, r_direction, r.time());

    if (!obj->hit(rotated_r, hit_rec, t_min, t_max)) return false;

    float3 p(costheta * hit_rec.p.x + sintheta * hit_rec.p.z, hit_rec.p.y,
             -sintheta * hit_rec.p.x + costheta * hit_rec.p.z);

    float3 n(costheta * hit_rec.n.x + sintheta * hit_rec.n.z, hit_rec.n.y,
             -sintheta * hit_rec.n.x + costheta * hit_rec.n.z);

    hit_rec.p = p;
    hit_rec.n = n;
    return true;
  }

  virtual bool bounding_box(float t0, float t1, AABB& output) const override {
    output = bbox;
    return hasbox;
  }

  virtual float3 center() const override { return obj->center(); }

  Brdf* getBrdf() const override { return obj->getBrdf(); }

  AABB bbox;
  bool hasbox;
  Geometry* obj;
  float rad, sintheta, costheta;
};