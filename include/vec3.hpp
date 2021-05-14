#pragma once
//
#include <debug.hpp>
#include <precision.hpp>
#include <utils.hpp>

using namespace pcarver;
namespace pcarver {

class vec3 {
public:
  vec3() : e{0, 0, 0} {}
  vec3(real e0, real e1, real e2) : e{e0, e1, e2} {}
  vec3(real e0) : e{e0, e0, e0} {}
  vec3(const real e1[3]) : e{e1[0], e1[1], e1[2]} {}

  real x() const { return e[0]; }
  real y() const { return e[1]; }
  real z() const { return e[2]; }
  real r() const { return x(); }
  real g() const { return y(); }
  real b() const { return z(); }
  real sum() const { return z() + x() + y(); }

  vec3 operator-() const {
    return vec3(-e[0], -e[1], -e[2]);
  }
  real operator[](int i) const { return e[i]; }
  real &operator[](int i) { return e[i]; }
  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }
  vec3 &operator*=(const real t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }
  vec3 &operator/=(const real t) { return *this *= 1 / t; }
  vec3 add(const vec3 &v) {
    return vec3(x() + v.x(), y() + v.y(), z() + v.z());
  }
  real sum() { return x() + y() + z(); }
  real average() { return sum() / 3.0; }
  vec3 add(const real &v) {
    return vec3(x() + v, y() + v, z() + v);
  }
  vec3 subt(const real &v) {
    return vec3(x() - v, y() - v, z() - v);
  }
  vec3 subt(const vec3 &v) {
    return vec3(x() - v.x(), y() - v.y(), z() - v.z());
  }
  vec3 multip(const vec3 &v) {
    return vec3(x() * v.x(), y() * v.y(), z() * v.z());
  }
  vec3 multip(const real &v) {
    return vec3(x() * v, y() * v, z() * v);
  }
  vec3 div(const real &v) {
    if (v == 0.0)
      throw std::runtime_error("no zero division");
    return vec3(x() / v, y() / v, z() / v);
  }
  vec3 div(const vec3 &v) {
    if (v.x() == 0.0)
      throw std::runtime_error(
          "no zero division x is zero");
    if (v.y() == 0.0)
      throw std::runtime_error(
          "no zero division y is zero");
    if (v.z() == 0.0)
      throw std::runtime_error(
          "no zero division z is zero");
    return vec3(x() / v.x(), y() / v.y(), z() / v.z());
  }
  real length() const { return sqrt(length_squared()); }
  real length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }
  inline real min() const {
    return fmin(fmin(x(), y()), z());
  }
  inline real max() const {
    return fmax(fmax(x(), y()), z());
  }
  bool near_zero() const {
    // Return true if the vector is close to zero in all
    // dimensions.
    const auto s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) &&
           (fabs(e[2]) < s);
  }

  std::vector<real> to_vector() const {
    std::vector<real> vdata(3);
    vdata[0] = x();
    vdata[1] = y();
    vdata[2] = z();
    return vdata;
  }
  void conditional_set(
      real setval, int index,
      const std::function<bool(real indval)> &fn) {
    if (fn(e[index])) {
      e[index] = setval;
    }
  }
  void conditional_set(
      real setval,
      const std::function<bool(real indval)> &fn) {
    for (int index = 0; index < 3; index++) {
      if (fn(e[index])) {
        e[index] = setval;
      }
    }
  }

public:
  real e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using tricolor = vec3; // 3d color

// vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out,
                                const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1],
              u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1],
              u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1],
              u.e[2] * v.e[2]);
}
inline vec3 operator*(real t, const vec3 &v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3 &v, real t) {
  return t * v;
}
inline vec3 operator/(vec3 v, real t) {
  return (1 / t) * v;
}
inline vec3 operator/(real t, vec3 v) {
  return (1 / t) * v;
}
inline vec3 operator/(vec3 v, vec3 t) {
  return (1 / t) * v;
}

inline real dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] +
         u.e[2] * v.e[2];
}
vec3 sqrt_vec(const vec3 &v) {
  auto r = sqrt(v.x());
  auto g = sqrt(v.y());
  auto b = sqrt(v.z());
  vec3 rgb = vec3(r, g, b);
  return rgb;
}
vec3 clamp(const vec3 &v, real mn, real mx) {
  auto r = clamp(v.x(), mn, mx);
  auto g = clamp(v.y(), mn, mx);
  auto b = clamp(v.z(), mn, mx);
  return vec3(r, g, b);
}
inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }
inline vec3 min_vec(const vec3 &v1, const vec3 &v2) {
  float x = fmin(v1.x(), v2.x());
  float y = fmin(v1.y(), v2.y());
  float z = fmin(v1.z(), v2.z());
  return vec3(x, y, z);
}
inline vec3 max_vec(const vec3 &v1, const vec3 &v2) {
  float x = fmax(v1.x(), v2.x());
  float y = fmax(v1.y(), v2.y());
  float z = fmax(v1.z(), v2.z());
  return vec3(x, y, z);
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
  return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n,
                    real etai_over_etat) {
  auto cos_theta = fmin(dot(-uv, n), 1.0);
  vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3 r_out_parallel =
      -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}
}
