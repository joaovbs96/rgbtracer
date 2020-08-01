#pragma once

#include "../math/onb.hpp"
#include "brdf.hpp"

class Lambertian : public Brdf {
 public:
  Lambertian(Texture* texture) : texture(texture) {}

  virtual float3 sample(const float3& p,   // next ray origin
                        const float3& wo,  // current ray direction
                        const float3& n,   // shading normal
                        uint& seed) const {
    ONB uvw(n);

    return uvw.local(sample_hemisphere(seed));
  }

  virtual float3 eval(const float3& p, const float3& wo, const float3& wi,
                      const float3& n, const float2 uv, float& pdf) const {
    float cosine = dot(normalize(wi), normalize(n));

    if (cosine < 0.f) {
      return float3(0.0f);
    } else {
      pdf = cosine / PI;
      return (cosine * texture->sample(uv, p)) / PI;
    }
  }

  virtual bool isSpecular() const override { return false; }
  virtual bool isLight() const override { return false; }

 private:
  Texture* texture;
};