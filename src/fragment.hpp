#ifndef FRAGMENT_HPP
#define FRAGMENT_HPP

#include "color.hpp"
#include "vector.hpp"

#include <cmath>

struct fragment_type {
  std::double_t z_depth;
  color_type color;
  vector_type normal;
  vector_type ray;
  vector_type position; // in world space
};

#endif
