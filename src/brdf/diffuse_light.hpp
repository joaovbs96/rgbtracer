#pragma once

#include "../math/onb.hpp"
#include "brdf.hpp"

class DiffuseLight : public Brdf {
 public:
  DiffuseLight(Texture* texture) : texture(texture) {}

  virtual float3 sample(const float3& p,   // next ray origin
                        const float3& wo,  // current ray direction
                        const float3& n,   // shading normal
                        uint& seed) const {
    return float3(0.0f);
  }

  virtual float3 eval(const float3& p, const float3& wo, const float3& wi,
                      const float3& n, const float2 uv, float& pdf) const {
    if (dot(n, wo) < 0.0f) {
      return texture->sample(uv, p);
    } else {
      return float3(0.0f);
    }
  }

  virtual bool isSpecular() const override { return false; }
  virtual bool isLight() const override { return true; }

 private:
  Texture* texture;
};