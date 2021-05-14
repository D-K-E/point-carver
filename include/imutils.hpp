#pragma once
// image utilities
#include <debug.hpp>
#include <precision.hpp>
#include <utils.hpp>
#include <vec3.hpp>

using namespace pcarver;

namespace pcarver {

typedef std::vector<std::vector<tricolor>> immat;

struct imcoord {
  row y;
  col x;
};

immat read_image(const std::string &img_path) {
  int w;
  int h;
  int nb_channels;
  unsigned char *data =
      stbi_load(img_path.c_str(), &w, &h, &nb_channels, 0);
  row height = static_cast<unsigned int>(h);
  col width = static_cast<unsigned int>(w);
  unsigned int bytes_per_scanline = nb_channels * width;
  immat img;
  for (row i = 0; i < height; i++) {
    std::vector<tricolor> row_vec;
    row_vec.resize(width);
    img.push_back(row_vec);
    for (col j = 0; j < width; j++) {
      auto pixel =
          data + i * bytes_per_scanline + j * nb_channels;
      tricolor tcolor(pixel[0], pixel[1], pixel[2]);
      img[i][j] = tcolor;
    }
  }
  return img;
}

struct ImgRect {
  immat pixels;
  imcoord bottom_left;
  ImgRect(const imcoord &coords, const immat &pixs)
      : pixels(pixs), bottom_left(coords) {}
  ImgRect(const std::string &impath) {
    pixels = read_image(impath);
    imcoord coord;
    coord.y = pixels.size();
    coord.x = 0;
    bottom_left = coord;
  }
  std::vector<tricolor> get_row(row index) const {

    COMP_CHECK_MSG(index > 0 || index < pixels.size(),
                   index, pixels.size(),
                   "given row index is out of bounds");
    return pixels[index];
  }
  std::vector<tricolor> get_col(col index) const {

    COMP_CHECK_MSG(index > 0 || index < pixels[0].size(),
                   index, pixels[0].size(),
                   "given column index is out of bounds");
    std::vector<tricolor> imcol(0, pixels.size());
    for (row i = 0; i < pixels.size(); i++) {
      auto rvec = pixels[i];
      imcol[i] = rvec[index];
    }
    return imcol;
  }
  tricolor get_pixel(row y, col x) const {
    auto rvec = get_row(y);
    COMP_CHECK_MSG(x > 0 || x < rvec.size(), x, rvec.size(),
                   "given column value is out of bounds");
    return rvec[x];
  }
  tricolor get_pixel(const imcoord &coord) const {
    return get_pixel(coord.y, coord.x);
  }
  real pixel_sum(row y, col x) const {
    auto pix = get_pixel(y, x);
    return pix.sum();
  }
  real sum() const {
    real s = 0;
    for (const auto &rvec : pixels) {
      for (const auto &cval : rvec) {
        s += cval.sum();
      }
    }
    return s;
  }

  std::vector<tricolor> rowsum() const {
    std::vector<tricolor> row_vec(0, pixels.size());
    for (row i = 0; i < pixels.size(); i++) {
      const auto &rvec = pixels[i];
      tricolor row_counter(0.0);
      for (const auto &cval : rvec) {
        row_counter += cval;
      }
      row_vec[i] = row_counter;
    }
    return row_vec;
  }
  std::vector<tricolor> colsum() const {
    std::vector<tricolor> col_vec(0, pixels[0].size());
    for (col j = 0; j < pixels[0].size(); j++) {
      tricolor col_counter(0.0);
      for (const auto &rvec : pixels) {
        col_counter += rvec[j];
      }
      col_vec[j] = col_counter;
    }
    return col_vec;
  }
  /**returns maximum pixel value in the image part*/
  tricolor max(row &y, col &x) {
    real temp = FLT_MIN;
    tricolor tval(0.0);
    for (row i = 0; i < pixels.size(); i++) {
      for (col j = 0; j < pixels[0].size(); j++) {
        tricolor cval = get_pixel(i, j);
        auto ctemp = cval.sum();
        if (ctemp > temp) {
          temp = ctemp;
          y = i;
          x = j;
          tval = cval;
        }
      }
    }
    return tval;
  }
  /**returns minimum pixel value in the image part*/
  tricolor min(row &y, col &x) {
    real temp = FLT_MAX;
    tricolor tval(0.0);
    for (row i = 0; i < pixels.size(); i++) {
      for (col j = 0; j < pixels[0].size(); j++) {
        tricolor cval = get_pixel(i, j);
        auto ctemp = cval.sum();
        if (ctemp < temp) {
          temp = ctemp;
          y = i;
          x = j;
          tval = cval;
        }
      }
    }
    return tval;
  }
  tricolor min() {
    row y = 0;
    col x = 0;
    return min(y, x);
  }
  tricolor max() {
    row y = 0;
    col x = 0;
    return max(y, x);
  }
  tricolor min(imcoord &coords) {
    row y = 0;
    col x = 0;
    auto val = min(y, x);
    coords.y = y;
    coords.x = x;
    return val;
  }
  tricolor max(imcoord &coords) {
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
