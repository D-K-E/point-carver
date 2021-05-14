#pragma once
// image utilities
#include <debug.hpp>
#include <precision.hpp>

using namespace pcarver;

namespace pcarver {

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

typedef std::vector<std::vector<real>> immat;

struct imcoord {
  row y;
  col x;
};

struct ImgRect {
  immat pixels;
  imcoord bottom_left;
  ImgRect(const imcoord &coords, const immat &pixs)
      : bottom_left(coords), pixels(pixs) {}
  std::vector<real> get_row(row index) const {

    COMP_CHECK_MSG(index > 0 || index < pixels.size(),
                   index, pixels.size(),
                   "given row index is out of bounds");
    return pixels[index];
  }
  std::vector<real> get_col(col index) const {

    COMP_CHECK_MSG(index > 0 || index < pixels[0].size(),
                   index, pixels[0].size(),
                   "given column index is out of bounds");
    std::vector<real> imcol(0, pixels.size());
    for (row i = 0; i < pixels.size(); i++) {
      auto rvec = pixels[i];
      imcol[i] = rvec[index];
    }
    return imcol;
  }
  real get_pixel(row y, col x) const {
    auto rvec = get_row(y);
    COMP_CHECK_MSG(x > 0 || x < rvec.size(), x, rvec.size(),
                   "given column value is out of bounds");
    return rvec[x];
  }
  real get_pixel(const imcoord &coord) const {
    return get_pixel(coord.y, coord.x);
  }

  std::vector<real> rowsum() const {
    std::vector<real> row_vec(0, pixels.size());
    for (row i = 0; i < pixels.size(); i++) {
      const auto &rvec = pixels[i];
      real row_counter = 0;
      for (const auto &cval : rvec) {
        row_counter += cval;
      }
      row_vec[i] = row_counter;
    }
    return row_vec;
  }
  std::vector<real> colsum() const {
    std::vector<real> col_vec(0, pixels[0].size());
    for (col j = 0; j < pixels[0].size(); j++) {
      real col_counter = 0;
      for (const auto &rvec : pixels) {
        col_counter += rvec[j];
      }
      col_vec[j] = col_counter;
    }
    return col_vec;
  }
  /**returns maximum pixel value in the image part*/
  real max(row &y, col &x) {
    real temp = FLT_MIN;
    for (row i = 0; i < pixels.size(); i++) {
      for (col j = 0; j < pixels[0].size(); j++) {
        real cval = get_pixel(i, j);
        if (cval > temp) {
          temp = cval;
          y = i;
          x = j;
        }
      }
    }
    return temp;
  }
  /**returns minimum pixel value in the image part*/
  real min(row &y, col &x) {
    real temp = FLT_MAX;
    for (row i = 0; i < pixels.size(); i++) {
      for (col j = 0; j < pixels[0].size(); j++) {
        real cval = get_pixel(i, j);
        if (cval < temp) {
          temp = cval;
          y = i;
          x = j;
        }
      }
    }
    return temp;
  }
  real min() {
    row y = 0;
    col x = 0;
    return min(y, x);
  }
  real max() {
    row y = 0;
    col x = 0;
    return max(y, x);
  }
  real min(imcoord &coords) {
    row y = 0;
    col x = 0;
    auto val = min(y, x);
    coords.y = y;
    coords.x = x;
    return val;
  }
  real max(imcoord &coords) {
    row y = 0;
    col x = 0;
    auto val = max(y, x);
    coords.y = y;
    coords.x = x;
    return val;
  }
  unsigned int nb_cols() const { return pixels[0].size(); }
  unsigned int nb_rows() const { return pixels.size(); }
  unsigned int nb_pixels() const {
    return nb_rows() * nb_cols();
  }
  unsigned int size() const { return nb_pixels(); }
  unsigned int width() const { return nb_cols(); }
  unsigned int height() const { return nb_rows(); }
};
};
