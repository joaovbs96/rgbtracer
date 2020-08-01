#pragma once

#include "../brdf/brdf.hpp"
#include "../light/light.hpp"
#include "../math/float3.hpp"
#include "../math/math.hpp"

// solves circular dependencies
class Brdf;
class Light;

// Scatter events
typedef enum {
  rayGotBounced,    // ray could get properly bounced, and is still alive
  rayGotCancelled,  // ray could not get scattered, and should get cancelled
  rayHitLight,      // ray hit a light, and should take emission into account
  rayMissed         // ray didn't hit anything, and went into the environemnt
} Scatter_Event;

struct Hit_Record {
  float t;
  float3 p;
  float3 n;
  float2 uv;
  Brdf* brdf;
  bool isSpecular;
};

struct Light_Record {
  float t;
  float3 p;
  float3 n;
  Light* light;
  bool isSpecular;
};

// Radiance PRD containing variables that should be propagated as the ray
// scatters.
struct Per_Ray_Data {
  float3 throughput = float3(1.0f);
  float3 radiance = float3(0.0f);
  Scatter_Event scatterEvent;
};