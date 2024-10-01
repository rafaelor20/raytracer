#pragma once

#include "Vec3.h"

using color = Vec3;

inline double linear_to_gamma(double linear_component) {
  if (linear_component > 0)
    return std::sqrt(linear_component);

  return 0;
}
