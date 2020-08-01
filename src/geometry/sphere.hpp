#pragma once

#include "../math/math.hpp"
#include "geometry.hpp"

class Sphere : public Geometry {
 public:
  Sphere(const float3& center, float radius, Brdf* material)
      : center_(center), radius_(radius), material_(material) {}

  bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
           float t_max) const override {
    // translates ray origin by sphere center
    const float3 oc = r.origin() - center_;

    const float a = dot(r.direction(), r.direction());
    const float b = dot(oc, r.direction()) * 2.0f;
    const float c = dot(oc, oc) - squared(radius_);
    const float d = squared(b) - 4.0f * a * c;

    // if the discriminant is < 0, no real roots exist
    if (d < 0.0f) return false;

    // checks if either real root falls in the valid range.

    // first real root
    float t = (-b - sqrtf(d)) / (2.0f * a);
    if (t >= t_min && t <= t_max) {
      hit_rec.t = t;
      hit_rec.p = r.point_at(t);
      hit_rec.n = normalize(hit_rec.p - center_);
      hit_rec.uv = sphere_uv(hit_rec.p);
      hit_rec.brdf = material_;
      return true;
    }

    // second real root
    t = (-b + sqrtf(d)) / (2.0f * a);
    if (t >= t_min && t <= t_max) {
      hit_rec.t = t;
      hit_rec.p = r.point_at(t);
      hit_rec.n = normalize(hit_rec.p - center_);
      hit_rec.uv = sphere_uv(hit_rec.p);
      hit_rec.brdf = material_;
      return true;
    }

    return false;
  }

  bool bounding_box(float t0, float t1, AABB& output) const override {
    output = AABB(center_ - float3(radius_), center_ + float3(radius_));

    return true;
  }

  float3 center() const { return center_; }

  Brdf* getBrdf() const override { return material_; }

  float3 sample(const float3& p,   // next ray origin
                const float3& wo,  // previous ray direction
                const float3& n,   // geometric normal
                uint& seed) const override {
    float r1 = rnd(seed);
    float r2 = rnd(seed);

    float d = squared_length(center_ - p);
    float z = 1.0f + r2 * (sqrtf(1.0f - squared(radius_) / d) - 1.0f);
    float zz = sqrtf(1.0f - squared(z));

    float phi = 2.0f * PI * r1;
    float3 wi = float3(cosf(phi) * zz, sinf(phi) * zz, z);

    ONB uvw(normalize(wi));

    return uvw.local(wi);
  }

  float pdf(const float3& p,   // origin of next ray
            const float3& wo,  // direction of current ray
            const float3& wi,  // direction of next ray
            const float3& n)   // geometric normal
      const override {
    if (!this->hit(Ray(p, wi), Hit_Record(), 0.001f, FLT_MAX)) return 0.0f;

    float d = squared_length(center_ - p);
    float cos_theta_max = sqrtf(1.0f - squared(radius_) / d);
    float solid_angle = 2.0f * PI * (1.0f - cos_theta_max);

    return 1.0f / solid_angle;
  }

 private:
  float3 center_;
  float radius_;
  Brdf* material_;
};