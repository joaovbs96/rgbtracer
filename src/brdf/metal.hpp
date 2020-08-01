#pragma once

#include "brdf.hpp"

class Metal : public Brdf {
 public:
  Metal(Texture* texture, const float fuzz)
      : texture(texture), fuzz_(saturate(fuzz)) {}

  virtual float3 sample(const float3& p, const float3& wo, const float3& n,
                        uint& seed) const {
    return reflect(-wo, n) + fuzz_ * random_in_unit_sphere(seed);
  }

  virtual float3 eval(const float3& p, const float3& wo, const float3& wi,
                      const float3& n, const float2 uv, float& pdf) const {
    return texture->sample(uv, p);
  }

  virtual bool isSpecular() const override { return true; }
  virtual bool isLight() const override { return false; }

 private:
  Texture* texture;
  float fuzz_;
};