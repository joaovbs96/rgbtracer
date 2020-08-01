#pragma once

#include "geometry.hpp"

// TODO: isn't working completely, investigate

class Motion : public Geometry {
 public:
  Motion();

  Motion(Geometry* object, const float3& pos, float t0, float t1)
      : obj_(object), p1_(pos), t0_(t0), t1_(t1) {}

  bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
           float t_max) const override {
    // get center position of the object
    float3 p0 = obj_->center();
    float3 center = p0 + ((r.time() - t0_) / (t1_ - t0_)) * (p1_ - p0);

    // translates ray origin by new center
    const Ray moved_ray = Ray(r.origin() - center, r.direction(), r.time());
    if (!obj_->hit(moved_ray, hit_rec, t_min, t_max)) return false;

    hit_rec.p += center;
    // TODO: change hit_rec.n?
    return true;
  }

  bool bounding_box(float t0, float t1, AABB& output) const override {
    if (!obj_->bounding_box(t0, t1, output)) return false;

    output = AABB(output.min() + p1_, output.max() + p1_);
    return true;
  }

  float3 center() const { return obj_->center(); }

  Brdf* getBrdf() const override { return obj_->getBrdf(); }

 private:
  Geometry* obj_;
  float3 p1_;
  float t0_, t1_;
};