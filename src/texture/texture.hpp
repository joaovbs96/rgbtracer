#pragma once

#include "../math/float3.hpp"

class Texture {
 public:
  virtual float3 sample(const float2& uv, const float3& p) const = 0;
};