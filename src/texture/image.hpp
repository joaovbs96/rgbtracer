#pragma once

#include <iostream>
#include "../include/stb_image.h"
#include "texture.hpp"

class Image : public Texture {
 public:
  Image() : data(nullptr), width(0), height(0) {}

  Image(const char* filename) {
    data = stbi_load(filename, &width, &height, &nn, 3);

    if (!data) {
      std::cerr << "Error loading image file '" << filename << std::endl;
    }
  }

  ~Image() { delete data; }

  virtual float3 sample(const float2& uv, const float3& p) const override {
    if (!data) return float3(0.0f, 1.0f, 1.0f);

    // Clamp input texture coordinates to [0,1] x [1,0]
    float nu = clamp(uv.u, 0.0f, 1.0f);
    float nv = 1.0f - clamp(uv.v, 0.0f, 1.0f);

    int i = (int)(nu * width);
    int j = (int)(nv * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    float r = float(data[3 * i + 3 * width * j]);
    float g = float(data[3 * i + 3 * width * j + 1]);
    float b = float(data[3 * i + 3 * width * j + 2]);

    return float3(r, g, b) / 255.0f;
  }

 private:
  unsigned char* data;
  int width, height, nn;
};