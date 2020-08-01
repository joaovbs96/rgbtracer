#pragma once

#include "geometry.hpp"

class XRect : public Geometry {
 public:
  XRect() {}

  XRect(float y0, float y1, float z0, float z1, float x, Brdf* material)
      : y0(y0), y1(y1), z0(z0), z1(z1), x(x), material(material) {
    c = float3(x, (y0 + y1) / 2.0f, (z0 + z1) / 2.0f);
  }

  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const override {
    float t = (x - r.origin().x) / r.direction().x;
    if (t < t_min || t > t_max) return false;

    float y = r.origin().y + t * r.direction().y;
    float z = r.origin().z + t * r.direction().z;
    if (y < y0 || y > y1 || z < z0 || z > z1) return false;

    hit_rec.uv = float2((y - y0) / (y1 - y0), (z - z0) / (z1 - z0));
    hit_rec.t = t;
    hit_rec.n = float3(1.0f, 0.0f, 0.0f);
    hit_rec.brdf = material;
    hit_rec.p = r.point_at(t);

    return true;
  }

  virtual bool bounding_box(float t0, float t1, AABB& output) const override {
    output = AABB(float3(x - 0.001f, y0, z0), float3(x + 0.001f, y1, z1));
    return true;
  }

  virtual float3 center() const override { return c; }
  Brdf* getBrdf() const override { return material; }

  float3 sample(const float3& p,   // next ray origin
                const float3& wo,  // previous ray direction
                const float3& n,   // geometric normal
                uint& seed) const override {
    float3 random_point = float3(x,                            // X
                                 y0 + rnd(seed) * (y1 - y0),   // Y
                                 z0 + rnd(seed) * (z1 - z0));  // Z

    return random_point - p;
  }

  float pdf(const float3& p,   // origin of next ray
            const float3& wo,  // direction of current ray
            const float3& wi,  // direction of next ray
            const float3& n)   // geometric normal
      const override {
    Hit_Record rec;
    if (!this->hit(Ray(p, wi), rec, 0.001f, FLT_MAX)) return 0.0f;

    float d = squared(rec.t) * squared_length(wi);
    float cosine = fabsf(dot(wi, rec.n)) / length(wi);
    float area = (y1 - y0) * (z1 - z0);

    return d / (cosine * area);
  }

  float x, y0, y1, z0, z1;
  float3 c;
  Brdf* material;
};

class YRect : public Geometry {
 public:
  YRect() {}

  YRect(float x0, float x1, float z0, float z1, float y, Brdf* material)
      : x0(x0), x1(x1), z0(z0), z1(z1), y(y), material(material) {
    c = float3((x0 + x1) / 2.0f, y, (z0 + z1) / 2.0f);
  }

  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const override {
    float t = (y - r.origin().y) / r.direction().y;
    if (t < t_min || t > t_max) return false;

    float x = r.origin().x + t * r.direction().x;
    float z = r.origin().z + t * r.direction().z;
    if (x < x0 || x > x1 || z < z0 || z > z1) return false;

    hit_rec.uv = float2((x - x0) / (x1 - x0), (z - z0) / (z1 - z0));
    hit_rec.t = t;
    hit_rec.n = float3(0.0f, 1.0f, 0.0f);
    hit_rec.brdf = material;
    hit_rec.p = r.point_at(t);

    return true;
  }

  virtual bool bounding_box(float t0, float t1, AABB& output) const override {
    output = AABB(float3(x0, y - 0.001f, z0), float3(x1, y + 0.001f, z1));
    return true;
  }

  virtual float3 center() const override { return c; }
  Brdf* getBrdf() const override { return material; }

  float3 sample(const float3& p,   // next ray origin
                const float3& wo,  // previous ray direction
                const float3& n,   // geometric normal
                uint& seed) const override {
    float3 random_point = float3(x0 + rnd(seed) * (x1 - x0),   // X
                                 y,                            // Y
                                 z0 + rnd(seed) * (z1 - z0));  // Z

    return random_point - p;
  }

  float pdf(const float3& p,   // origin of next ray
            const float3& wo,  // direction of current ray
            const float3& wi,  // direction of next ray
            const float3& n)   // geometric normal
      const override {
    Hit_Record rec;
    if (!this->hit(Ray(p, wi), rec, 0.001f, FLT_MAX)) return 0.0f;

    float d = squared(rec.t) * squared_length(wi);
    float cosine = fabsf(dot(wi, rec.n)) / length(wi);
    float area = (x1 - x0) * (z1 - z0);

    return d / (cosine * area);
  }

  float x0, x1, y, z0, z1;
  float3 c;
  Brdf* material;
};

class ZRect : public Geometry {
 public:
  ZRect() {}

  ZRect(float x0, float x1, float y0, float y1, float z, Brdf* material)
      : x0(x0), x1(x1), y0(y0), y1(y1), z(z), material(material) {
    c = float3((x0 + x1) / 2.0f, (y0 + y1) / 2.0f, z);
  }

  virtual bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
                   float t_max) const override {
    float t = (z - r.origin().z) / r.direction().z;
    if (t < t_min || t > t_max) return false;

    float x = r.origin().x + t * r.direction().x;
    float y = r.origin().y + t * r.direction().y;
    if (x < x0 || x > x1 || y < y0 || y > y1) return false;

    hit_rec.uv = float2((x - x0) / (x1 - x0), (y - y0) / (y1 - y0));
    hit_rec.t = t;
    hit_rec.n = float3(0.0f, 0.0f, 1.0f);
    hit_rec.brdf = material;
    hit_rec.p = r.point_at(t);

    return true;
  }

  virtual bool bounding_box(float t0, float t1, AABB& output) const override {
    output = AABB(float3(x0, y0, z - 0.001f), float3(x1, y1, z + 0.001f));
    return true;
  }

  virtual float3 center() const override { return c; }
  Brdf* getBrdf() const override { return material; }

  float3 sample(const float3& p,   // next ray origin
                const float3& wo,  // previous ray direction
                const float3& n,   // geometric normal
                uint& seed) const override {
    float3 random_point = float3(x0 + rnd(seed) * (x1 - x0),  // X
                                 y0 + rnd(seed) * (y1 - y0),  // Y
                                 z);                          // Z

    return random_point - p;
  }

  float pdf(const float3& p,   // origin of next ray
            const float3& wo,  // direction of current ray
            const float3& wi,  // direction of next ray
            const float3& n)   // geometric normal
      const override {
    Hit_Record rec;
    if (!this->hit(Ray(p, wi), rec, 0.001f, FLT_MAX)) return 0.0f;

    float d = squared(rec.t) * squared_length(wi);
    float cosine = fabsf(dot(wi, rec.n)) / length(wi);
    float area = (x1 - x0) * (y1 - y0);

    return d / (cosine * area);
  }

  float x0, x1, y0, y1, z;
  float3 c;
  Brdf* material;
};