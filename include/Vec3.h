#pragma once

#include "rtweekend.h"
#include <cmath>
#include <iostream>
#include <ostream>

class Vec3 {
public:
  Vec3();
  Vec3(double e0, double e1, double e2);

  double x() const;
  double y() const;
  double z() const;

  Vec3 operator-() const;
  double operator[](int i) const;
  double &operator[](int i);
  Vec3 &operator+=(const Vec3 &v);
  Vec3 &operator*=(double t);
  Vec3 &operator/=(double t);
  double length() const;
  double length_squared() const;
  bool near_zero() const;
  friend std::ostream &operator<<(std::ostream &out, const Vec3 &v);
  friend Vec3 operator+(const Vec3 &u, const Vec3 &v);
  friend Vec3 operator-(const Vec3 &u, const Vec3 &v);
  friend Vec3 operator*(const Vec3 &u, const Vec3 &v);
  friend Vec3 operator*(double t, const Vec3 &v);
  friend Vec3 operator*(const Vec3 &v, double t);
  friend Vec3 operator/(const Vec3 &v, double t);
  static Vec3 random() {
    return Vec3(random_double(), random_double(), random_double());
  }

  static Vec3 random(double min, double max) {
    return Vec3(random_double(min, max), random_double(min, max),
                random_double(min, max));
  }
  static double dot(const Vec3 &u, const Vec3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
  }

  static Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
  }

  static Vec3 unit_vector(const Vec3 &v) { return v / v.length(); }

  static Vec3 random_unit_vector() {
    while (true) {
      auto p = Vec3::random(-1, 1);
      auto lensq = p.length_squared();
      if (1e-160 < lensq && lensq <= 1)
        return p / sqrt(lensq);
    }
  }

  static Vec3 random_on_hemisphere(const Vec3 &normal) {
    Vec3 on_unit_sphere = random_unit_vector();
    if (Vec3::dot(on_unit_sphere, normal) > 0.0)
      return on_unit_sphere;
    else
      return -on_unit_sphere;
  }

  static Vec3 sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
    // square.
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
  }

  static Vec3 reflect(const Vec3 &v, const Vec3 &n) {
    return v - 2 * dot(v, n) * n;
  }

  static Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel =
        -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
  }

  static Vec3 random_in_unit_disk() {
    while (true) {
      auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
      if (p.length_squared() < 1)
        return p;
    }
  }

private:
  double e[3];
};

using point3 = Vec3;
