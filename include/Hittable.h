#pragma once
#include "HitRecord.h"
#include "interval.h"

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(const Ray &r, interval ray_t, HitRecord &rec) const = 0;
};
