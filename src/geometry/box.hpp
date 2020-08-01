#pragma once

#include "flip_normals.hpp"
#include "geometry_list.hpp"
#include "rectangle.hpp"

class Box : public Geometry {
 public:
  Box() {}
  Box(const float3& p0, const float3& p1, Brdf* material)
      : p0(p0), p1(p1), material(material) {
    center_ = (p0 + p1) / 2.0f;

    sides.add(new ZRect(p0.x, p1.x, p0.y, p1.y, p1.z, material));
    sides.add(new Flip(new ZRect(p0.x, p1.x, p0.y, p1.y, p0.z, material)));

    sides.add(new YRect(p0.x, p1.x, p0.z, p1.z, p1.y, material));
    sides.add(new Flip(new YRect(p0.x, p1.x, p0.z, p1.z, p0.y, material)));

    sides.add(new XRect(p0.y, p1.y, p0.z, p1.z, p1.x, material));
    sides.add(new Flip(new XRect(p0.y, p1.y, p0.x, p1.x, p0.x, material)));
  }

  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const override {
    return sides.hit(r, hit_rec, t_min, t_max);
  }

  virtual bool bounding_box(float t0, float t1, AABB& output) const override {
    output = AABB(p0, p1);
    return true;
  }

  virtual float3 center() const override { return center_; }

  Brdf* getBrdf() const override { return material; }

  float3 p0, p1, center_;
  GeometryList sides;
  Brdf* material;
};