#pragma once

#include "Ray.h"
#include <memory>

class Material;

class HitRecord {
public:
  point3 p;
  Vec3 normal;
  std::shared_ptr<Material> mat;
  double t;
  bool front_face;

  void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
    front_face = Vec3::dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};
