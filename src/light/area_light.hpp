#pragma once

#include "light.hpp"

class AreaLight : public Light {
 public:
  AreaLight();
  AreaLight(Geometry* object) : obj_(object), light_(object->getBrdf()) {}

  virtual bool hit(const Ray& r, Light_Record& light_rec, float t_min,
                   float t_max) const override {
    Hit_Record geom_rec;

    if (!obj_->hit(r, geom_rec, t_min, t_max)) return false;

    light_rec.t = geom_rec.t;
    light_rec.p = geom_rec.p;
    light_rec.n = geom_rec.n;
    light_rec.isSpecular = false;

    return true;
  }

  virtual float3 sample(const float3& p, const float3& wo, const float3& n,
                        uint& seed) const {
    return obj_->sample(p, wo, n, seed);
  }

  virtual float pdf(const float3& p, const float3& wo, const float3& wi,
                    const float3& n) const {
    return obj_->pdf(p, wo, wi, n);
  }

  virtual float3 emission(const float3& p, const float3& wo, const float3& wi,
                          const float3& n, const float2 uv,
                          float& pdf) const override {
    return light_->eval(p, wo, wi, n, uv, pdf);
  }

  virtual Geometry* getGeometry() const override { return obj_; }

  virtual Brdf* getBrdf() const override { return light_; }

 private:
  Geometry* obj_;
  Brdf* light_;
};