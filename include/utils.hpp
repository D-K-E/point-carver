#pragma once

#include <external.hpp>
#include <precision.hpp>

namespace pcarver {

/**
  \brief transform degree to radian
 */
inline real degrees_to_radians(real degrees) {
  return degrees * M_PI / 180.0;
}
/**
  \brief transform radian to degrees
 */
inline real radians_to_degrees(real radian) {
  return radian * 180.0 / M_PI;
}

/**
  \brief clamp value to given range
  \param x value to be clamped
  \param min minimum value of the range
  \param max maximum value of the range

  \return either x, min or the max
 */
template <typename T, typename U, typename V>
T clamp(T x, U min, V max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

/**
  \brief clamp value to given range
  \param x value to be clamped
  \param min minimum value of the range
  \param max maximum value of the range

  \return either x, min or the max all in the same type
 */
template <typename T> T dclamp(T x, T mn, T mx) {
  return clamp<T, T, T>(x, mn, mx);
}

/**
  \brief even spaced vector between given range

  \param start start of the range
  \param end end of the range
  \param size of the resulting vector

  \return lspaced evenly spaced vector
 */
template <typename T>
std::vector<T> linspace(T start, T end, uint size) {
  // from: https://stackoverflow.com/a/27030598/7330813
  std::vector<T> lspaced;
  if (size == 0)
    return lspaced;
  if (size == 1) {
    lspaced.push_back(start);
    return lspaced;
  }
  auto delta = (end - start) / (size - 1);
  for (unsigned int i = 0; i < size - 1; i++) {
    lspaced.push_back(start + delta * i);
  }
  lspaced.push_back(end);
  return lspaced;
}

/**
  \brief cast vector values to DestType type

  \param vs vector whose values are going to be cast
  \param fn cast, transform function

  \return ds vector in DestType
 */
template <typename SrcType, typename DestType>
inline std::vector<DestType>
cast_vec(const std::vector<SrcType> &vs,
         const std::function<DestType(SrcType)> &fn) {
  std::vector<DestType> ds;
  ds.resize(vs.size());
  for (std::size_t i = 0; i < vs.size(); i++) {
    ds[i] = fn(vs[i]);
  }
  return ds;
}

/**
  \brief cast vector values to DestType type

  \param vs vector whose values are going to be cast
  \param fn cast, transform function

  \return vector in DestType
 */
template <typename SrcType, typename DestType>
inline std::vector<DestType>
cast_vec(const std::vector<SrcType> &vs) {
  return cast_vec<SrcType, DestType>(
      vs, [](auto i) { return static_cast<DestType>(i); });
}
}
