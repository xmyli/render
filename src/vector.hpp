#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "configuration.hpp"

#include <cmath>
#include <numbers>

class vector_type {
public:
  vector_type() = default;

  vector_type(std::double_t x_axis_value, std::double_t y_axis_value,
              std::double_t z_axis_value)
      : x_axis_value(x_axis_value), y_axis_value(y_axis_value),
        z_axis_value(z_axis_value) {}

  auto operator+(const vector_type &other) const -> vector_type {
    return vector_type{this->x_axis_value + other.x_axis_value,
                       this->y_axis_value + other.y_axis_value,
                       this->z_axis_value + other.z_axis_value};
  }

  auto operator-(const vector_type &other) const -> vector_type {
    return vector_type{this->x_axis_value - other.x_axis_value,
                       this->y_axis_value - other.y_axis_value,
                       this->z_axis_value - other.z_axis_value};
  }

  auto operator*(const int &value) const -> vector_type {
    return vector_type{this->x_axis_value * value, this->y_axis_value * value,
                       this->z_axis_value * value};
  }

  auto operator*(const std::float_t &value) const -> vector_type {
    return vector_type{this->x_axis_value * value, this->y_axis_value * value,
                       this->z_axis_value * value};
  }

  auto operator*(const std::double_t &value) const -> vector_type {
    return vector_type{this->x_axis_value * value, this->y_axis_value * value,
                       this->z_axis_value * value};
  }

  [[nodiscard]] auto dot_product(const vector_type &other) const
      -> std::double_t {
    return this->x_axis_value * other.get_x() +
           this->y_axis_value * other.get_y() +
           this->z_axis_value * other.get_z();
  }

  [[nodiscard]] auto cross_product(const vector_type &other) const
      -> vector_type {
    auto x_axis_value =
        this->y_axis_value * other.get_z() - this->z_axis_value * other.get_y();
    auto y_axis_value = -this->x_axis_value * other.get_z() +
                        this->z_axis_value * other.get_x();
    auto z_axis_value =
        this->x_axis_value * other.get_y() - this->y_axis_value * other.get_x();
    return {x_axis_value, y_axis_value, z_axis_value};
  }

  [[nodiscard]] auto get_x() const -> std::double_t {
    return this->x_axis_value;
  }

  auto set_x(std::double_t value) -> void { this->x_axis_value = value; }

  [[nodiscard]] auto get_y() const -> std::double_t {
    return this->y_axis_value;
  }

  auto set_y(std::double_t value) -> void { this->y_axis_value = value; }

  [[nodiscard]] auto get_z() const -> std::double_t {
    return this->z_axis_value;
  }

  auto set_z(std::double_t value) -> void { this->z_axis_value = value; }

  [[nodiscard]] auto get_length() const -> std::double_t {
    return std::sqrt(this->x_axis_value * this->x_axis_value +
                     this->y_axis_value * this->y_axis_value +
                     this->z_axis_value * this->z_axis_value);
  }

  [[nodiscard]] auto get_normalized() const -> vector_type {
    return {this->x_axis_value / this->get_length(),
            this->y_axis_value / this->get_length(),
            this->z_axis_value / this->get_length()};
  }

private:
  std::double_t x_axis_value;
  std::double_t y_axis_value;
  std::double_t z_axis_value;
};

#endif
