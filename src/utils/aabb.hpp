#pragma once

#include "../math/float3.hpp"
#include "ray.hpp"

class AABB {
 public:
  AABB() {}
  AABB(const float3& a, const float3& b) : _min(a), _max(b) {}

  float3 min() const { return _min; }
  float3 max() const { return _max; }

  bool hit(const Ray& r, float tmin, float tmax) const {
    float3 t0 = (_min - r.origin()) / r.direction();
    float3 t1 = (_max - r.origin()) / r.direction();
    float3 t_min = min_float3(t0, t1);  // pair-wise min vector
    float3 t_max = max_float3(t0, t1);  // pair-wise max vector

    return max_component(t_min) <= min_component(t_max);
  }

 private:
  float3 _min;
  float3 _max;
};

AABB surrounding_box(const AABB& box0, const AABB& box1) {
  float3 small = min_float3(box0.min(), box1.min());
  float3 big = max_float3(box0.max(), box1.max());

  return AABB(small, big);
}