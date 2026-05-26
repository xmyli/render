#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "color.hpp"
#include "vector.hpp"

#include <cmath>

struct light_type {
  vector_type position;
  color_type color;
  std::double_t intensity;
};

#endif
