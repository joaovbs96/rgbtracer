#pragma once

#include "texture.hpp"

class Color : public Texture {
 public:
  Color();
  Color(float3 c) : color(c) {}
  Color(float r, float g, float b) : color(float3(r, g, b)) {}

  virtual float3 sample(const float2& uv, const float3& p) const override {
    return color;
  }

 private:
  float3 color;
};