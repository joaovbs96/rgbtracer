#pragma once

#include <algorithm>
#include <functional>
#include <iostream>

#include "geometry.hpp"
#include "geometry_list.hpp"

// box comprator functions defines for readability
typedef std::function<bool(Geometry*, Geometry*)> Comparator;
typedef std::vector<Comparator> Comparators;

bool box_x_compare(Geometry* a, Geometry* b) {
  AABB box_a;
  AABB box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min().x < box_b.min().x;
}

bool box_y_compare(Geometry* a, Geometry* b) {
  AABB box_a;
  AABB box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min().y < box_b.min().y;
}

bool box_z_compare(Geometry* a, Geometry* b) {
  AABB box_a;
  AABB box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min().z < box_b.min().z;
}

class BVHNode : public Geometry {
 public:
  BVHNode();

  BVHNode(GeometryList& list, float time0, float time1)
      : BVHNode(list, 0, list.size(), time0, time1, 0) {}

  BVHNode(GeometryList& list, int start, int end, float time0, float time1,
          int axis) {
    int span = end - start;

    if (span == 1) {
      left_node = right_node = list[start];
    } else if (span == 2) {
      left_node = list[start];
      right_node = list[start + 1];
    } else {
      Comparators compare{box_x_compare, box_y_compare, box_z_compare};
      std::sort(list.begin() + start, list.begin() + end, compare[axis]);

      auto mid = start + span / 2;
      int next_axis = (axis + 1) % 3;
      left_node = new BVHNode(list, start, mid, time0, time1, next_axis);
      right_node = new BVHNode(list, mid, end, time0, time1, next_axis);
    }

    AABB box_left, box_right;
    if (!left_node->bounding_box(time0, time1, box_left) ||
        !right_node->bounding_box(time0, time1, box_right))
      std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surrounding_box(box_left, box_right);
  }

  bool hit(const Ray& r, Hit_Record& hit_rec, float t_min, float t_max) const {
    if (!box.hit(r, t_min, t_max)) return false;

    bool left = left_node->hit(r, hit_rec, t_min, t_max);
    bool right = right_node->hit(r, hit_rec, t_min, left ? hit_rec.t : t_max);

    return left || right;
  }

  bool bounding_box(float t0, float t1, AABB& output) const {
    output = box;
    return true;
  }

  // center of a BVH node is the mean of the BVH leafs
  float3 center() const {
    return (left_node->center() + right_node->center()) / 2.0f;
  }

  // when asked for a BRDF, return the brdf of the left node
  Brdf* getBrdf() const override { return left_node->getBrdf(); }

  Geometry *left_node, *right_node;
  AABB box;
};