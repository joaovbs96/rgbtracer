#pragma once

#include "../math/float3.hpp"

class Ray {
 public:
  Ray(const float3& o, const float3& d, const float& time = 0.0f)
      : origin_(o), direction_(normalize(d)), time_(time) {}

  const float3& origin() const { return origin_; }
  const float3& direction() const { return direction_; }

  const float time() const { return time_; }

  float3 point_at(const float t) const { return origin_ + t * direction_; }

 private:
  float time_;
  float3 origin_;
  float3 direction_;
};