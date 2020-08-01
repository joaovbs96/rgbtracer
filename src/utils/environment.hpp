#pragma once

#include "../utils/ray.hpp"

class Environment {
 public:
  virtual float3 sample(const Ray& r) const = 0;
};

class GradientBackground : public Environment {
 public:
  GradientBackground(const float3& color1, const float3& color2)
      : color1(color1), color2(color2) {}

  float3 sample(const Ray& r) const {
    float t = 0.5f * (r.direction().y + 1.0f);
    return (1.0f - t) * color1 + t * color2;
  }

  float3 color1;
  float3 color2;
};

class ColorBackground : public Environment {
 public:
  ColorBackground(const float3& color) : color(color) {}

  float3 sample(const Ray& r) const { return color; }

  float3 color;
};