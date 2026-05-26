#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <cmath>
#include <numbers>

constexpr auto width = 900;
constexpr auto height = 900;

constexpr auto fragment_width = std::double_t{2.0} / width;
constexpr auto fragment_height = std::double_t{2.0} / height;

constexpr std::double_t camera_rotation_speed = 1;
constexpr std::double_t camera_movement_speed = 0.025;

constexpr std::double_t specular_exponent = 32;

// CONSTANTS

constexpr std::double_t radian_to_degree_conversion_factor =
    180 / std::numbers::pi;
constexpr std::double_t degree_to_radian_conversion_factor =
    std::numbers::pi / 180;

#endif
