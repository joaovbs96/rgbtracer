#pragma once

#include "../sampling/rng.hpp"
#include "../sampling/sampling.hpp"
#include "../utils/camera.hpp"
#include "../utils/ray.hpp"
#include "../utils/scenes.hpp"

class Integrator {
 public:
  virtual float3 render(Scene& scene, const int& c, const int& r,
                        uint& seed) = 0;
};