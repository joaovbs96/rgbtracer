#pragma once

#include "../utils/prd.hpp"
#include "../utils/ray.hpp"

// solves circular dependency
struct Light_Record;
class Geometry;
class Brdf;

class Light {
 public:
  virtual float3 emission(const float3& p, const float3& wo, const float3& wi,
                          const float3& n, const float2 uv,
                          float& pdf) const = 0;

  virtual bool hit(const Ray& r, Light_Record& light_rec, float t_min,
                   float t_max) const = 0;

  virtual float3 sample(const float3& p, const float3& wo, const float3& n,
                        uint& seed) const = 0;

  virtual float pdf(const float3& p, const float3& wo, const float3& wi,
                    const float3& n) const = 0;

  virtual Geometry* getGeometry() const = 0;

  virtual Brdf* getBrdf() const = 0;
};