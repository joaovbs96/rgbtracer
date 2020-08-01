#pragma once

#include <vector>

#include "geometry.hpp"

// list initialization taken from @nice_byte's implementation
// https://github.com/nicebyte/ayanami

class GeometryList : public Geometry {
 public:
  template <class... Args>
  GeometryList(Args... args) : list_{std::forward<Args>(args)...} {}

  GeometryList(GeometryList& geom_list) : list_(geom_list.get_list()) {}

  bool hit(const Ray& r, Hit_Record& hit_rec, float t_min,
           float t_max) const override {
    Hit_Record list_rec;  // temp hit record
    bool anyhit = false;
    float closest_hit = t_max;

    // iterates over list of geometries to find the closest intersection
    for (const auto& geom : list_) {
      if (geom->hit(r, list_rec, t_min, closest_hit)) {
        anyhit = true;
        closest_hit = list_rec.t;
        hit_rec = list_rec;
      }
    }

    return anyhit;
  }

  // append new geometry to list of objects
  void add(Geometry* geom) { list_.push_back(geom); }

  std::vector<Geometry*> get_list() { return list_; }

  // returns specific geometry
  Geometry*& operator[](int index) { return list_[index]; }

  // begin and end pointers
  auto begin() { return list_.begin(); }
  auto end() { return list_.end(); }

  // returns number of geometries in list
  int size() { return (int)list_.size(); }

  bool bounding_box(float t0, float t1, AABB& output) const override {
    if (list_.empty()) return false;

    AABB temp_box;
    bool first_box = true;

    for (const auto& object : list_) {
      if (!object->bounding_box(t0, t1, temp_box)) return false;
      output = first_box ? temp_box : surrounding_box(output, temp_box);
      first_box = false;
    }

    return true;
  }

  // center of a list is the mean of the center of all its objects
  float3 center() const {
    float3 c = float3(0.0f);
    for (const auto& object : list_) {
      c += object->center();
    }

    return c / (float)list_.size();
  }

  // when asked for a BRDF, return the brdf of the first element
  Brdf* getBrdf() const override { return list_[0]->getBrdf(); }

 private:
  std::vector<Geometry*> list_;
};