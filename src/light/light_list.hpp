#pragma once

#include <vector>
#include "light.hpp"

// differently from GeometryList, this is just a container, not a light per se
class LightList {
 public:
  template <class... Args>
  LightList(Args... args) : list_{std::forward<Args>(args)...} {}

  LightList(LightList& light_list) : list_(light_list.get_list()) {}

  bool hit(const Ray& r, Light_Record& light_rec, float t_min,
           float t_max) const {
    Light_Record list_rec;  // temp hit record
    bool anyhit = false;
    float closest_hit = t_max;

    // check if list is empty
    if (list_.empty()) return false;

    // iterates over list of lights to find the closest intersection
    for (const auto& light : list_) {
      if (light->hit(r, list_rec, t_min, closest_hit)) {
        anyhit = true;
        closest_hit = list_rec.t;
        light_rec = list_rec;
        light_rec.light = light;
      }
    }

    return anyhit;
  }

  virtual float3 sample(const float3& p, const float3& wo, const float3& n,
                        uint& seed) const {
    int i = (int)floorf(rnd(seed) * list_.size());

    return normalize(list_[i]->sample(p, wo, n, seed));
  }

  virtual float pdf(const float3& p, const float3& wo, const float3& wi,
                    const float3& n) const {
    float pdf = 0;

    for (int i = 0; i < list_.size(); i++)
      pdf += list_[i]->pdf(p, wo, wi, n) / list_.size();

    return pdf;
  }

  // append new light to list of lights
  void add(Light* light) { list_.push_back(light); }

  std::vector<Light*> get_list() { return list_; }

  // returns specific light
  Light*& operator[](int index) { return list_[index]; }

  // begin and end pointers
  auto begin() { return list_.begin(); }
  auto end() { return list_.end(); }

  // returns number of lights in list
  int size() { return (int)(list_.size()); }

 private:
  std::vector<Light*> list_;
};