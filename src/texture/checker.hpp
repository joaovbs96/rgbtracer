#pragma once

#include "texture.hpp"

class Checker : public Texture {
 public:
  Checker();
  Checker(Texture* t0, Texture* t1) : odd(t0), even(t1) {}

  virtual float3 sample(const float2& uv, const float3& p) const override {
    float sines = sinf(10.0f * p.x) * sinf(10.0f * p.y) * sinf(10.0f * p.z);
    return sines < 0.0f ? odd->sample(uv, p) : even->sample(uv, p);
  }

 private:
  Texture* odd;
  Texture* even;
};