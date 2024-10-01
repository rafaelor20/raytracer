#include "Vec3.h"

Vec3::Vec3() : e{0, 0, 0} {}
Vec3::Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

double Vec3::x() const { return e[0]; }

double Vec3::y() const { return e[1]; }

double Vec3::z() const { return e[2]; }

double Vec3::length() const { return std::sqrt(length_squared()); }

double Vec3::length_squared() const {
  return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

bool Vec3::near_zero() const {
  // Return true if the vector is close to zero in all dimensions.
  auto s = 1e-8;
  return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) &&
         (std::fabs(e[2]) < s);
}

Vec3 Vec3::operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

double Vec3::operator[](int i) const { return e[i]; }

double &Vec3::operator[](int i) { return e[i]; }

Vec3 &Vec3::operator+=(const Vec3 &v) {
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];
  return *this;
}

Vec3 &Vec3::operator*=(double t) {
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;
  return *this;
}

Vec3 &Vec3::operator/=(double t) { return *this *= 1 / t; }

std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

Vec3 operator*(double t, const Vec3 &v) {
  return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

Vec3 operator*(const Vec3 &v, double t) { return t * v; }

Vec3 operator/(const Vec3 &v, double t) { return (1 / t) * v; }
