#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "configuration.hpp"
#include "vector.hpp"

#include <optional>

template <int order> class matrix_type;

template <> class matrix_type<2> {
public:
  matrix_type(std::double_t value_1_1, std::double_t value_1_2,
              std::double_t value_2_1, std::double_t value_2_2)
      : value_1_1(value_1_1), value_1_2(value_1_2), value_2_1(value_2_1),
        value_2_2(value_2_2) {}

  auto operator*(const vector_type &vector) const -> vector_type {
    auto new_x =
        this->value_1_1 * vector.get_x() + this->value_1_2 * vector.get_y();
    auto new_y =
        this->value_2_1 * vector.get_x() + this->value_2_2 * vector.get_y();
    return {new_x, new_y, vector.get_z()};
  }

  [[nodiscard]] auto get_determinant() const -> std::double_t {
    return value_1_1 * value_2_2 - value_1_2 * value_2_1;
  }

  [[nodiscard]] auto get_inversion() const -> std::optional<matrix_type> {
    auto determinant = this->get_determinant();
    if (determinant <= 0) {
      return std::nullopt;
    }
    auto inversion_value_1_1 = value_2_2 / determinant;
    auto inversion_value_1_2 = -value_1_2 / determinant;
    auto inversion_value_2_1 = -value_2_1 / determinant;
    auto inversion_value_2_2 = value_1_1 / determinant;
    return std::make_optional<matrix_type>(
        inversion_value_1_1, inversion_value_1_2, inversion_value_2_1,
        inversion_value_2_2);
  }

private:
  std::double_t value_1_1;
  std::double_t value_1_2;
  std::double_t value_2_1;
  std::double_t value_2_2;
};

template <> class matrix_type<3> {
public:
  matrix_type(std::double_t value_1_1, std::double_t value_1_2,
              std::double_t value_1_3, std::double_t value_2_1,
              std::double_t value_2_2, std::double_t value_2_3,
              std::double_t value_3_1, std::double_t value_3_2,
              std::double_t value_3_3)
      : value_1_1(value_1_1), value_1_2(value_1_2), value_1_3(value_1_3),
        value_2_1(value_2_1), value_2_2(value_2_2), value_2_3(value_2_3),
        value_3_1(value_3_1), value_3_2(value_3_2), value_3_3(value_3_3) {}

  matrix_type() = default;

  auto operator*(vector_type &vector) const -> vector_type {
    auto new_x = this->value_1_1 * vector.get_x() +
                 this->value_1_2 * vector.get_y() +
                 this->value_1_3 * vector.get_z();
    auto new_y = this->value_2_1 * vector.get_x() +
                 this->value_2_2 * vector.get_y() +
                 this->value_2_3 * vector.get_z();
    auto new_z = this->value_3_1 * vector.get_x() +
                 this->value_3_2 * vector.get_y() +
                 this->value_3_3 * vector.get_z();
    return {new_x, new_y, new_z};
  }

  [[nodiscard]] auto get_determinant() const -> std::double_t {
    return value_1_1 * (value_2_2 * value_3_3 - value_2_3 * value_3_2) -
           value_1_2 * (value_2_1 * value_3_3 - value_2_3 * value_3_1) +
           value_1_3 * (value_2_1 * value_3_2 - value_2_2 * value_3_1);
  }

  [[nodiscard]] auto get_inversion() const -> std::optional<matrix_type> {
    auto determinant = this->get_determinant();
    if (determinant <= 0) {
      return std::nullopt;
    }
    auto inversion_value_1_1 =
        (value_2_2 * value_3_3 - value_2_3 * value_3_2) / determinant;
    auto inversion_value_1_2 =
        -(value_1_2 * value_3_3 - value_1_3 * value_3_2) / determinant;
    auto inversion_value_1_3 =
        (value_1_2 * value_2_3 - value_1_3 * value_2_2) / determinant;
    auto inversion_value_2_1 =
        -(value_2_1 * value_3_3 - value_2_3 * value_3_1) / determinant;
    auto inversion_value_2_2 =
        (value_1_1 * value_3_3 - value_1_3 * value_3_1) / determinant;
    auto inversion_value_2_3 =
        -(value_1_1 * value_2_3 - value_1_3 * value_2_1) / determinant;
    auto inversion_value_3_1 =
        (value_2_1 * value_3_2 - value_2_2 * value_3_1) / determinant;
    auto inversion_value_3_2 =
        -(value_1_1 * value_3_2 - value_1_2 * value_3_1) / determinant;
    auto inversion_value_3_3 =
        (value_1_1 * value_2_2 - value_1_2 * value_2_1) / determinant;
    return std::make_optional<matrix_type>(
        inversion_value_1_1, inversion_value_1_2, inversion_value_1_3,
        inversion_value_2_1, inversion_value_2_2, inversion_value_2_3,
        inversion_value_3_1, inversion_value_3_2, inversion_value_3_3);
  }

  static auto get_rotation_matrix(std::double_t angle, vector_type axis)
      -> matrix_type<3> {
    angle = angle * degree_to_radian_conversion_factor;
    axis = axis.get_normalized();
    auto cosine = std::cos(angle);
    auto sine = std::sin(angle);
    auto value_1_1 = axis.get_x() * axis.get_x() * (1 - cosine) + cosine;
    auto value_1_2 =
        axis.get_x() * axis.get_y() * (1 - cosine) - axis.get_z() * sine;
    auto value_1_3 =
        axis.get_x() * axis.get_z() * (1 - cosine) + axis.get_y() * sine;
    auto value_2_1 =
        axis.get_x() * axis.get_y() * (1 - cosine) + axis.get_z() * sine;
    auto value_2_2 = axis.get_y() * axis.get_y() * (1 - cosine) + cosine;
    auto value_2_3 =
        axis.get_y() * axis.get_z() * (1 - cosine) - axis.get_x() * sine;
    auto value_3_1 =
        axis.get_x() * axis.get_z() * (1 - cosine) - axis.get_y() * sine;
    auto value_3_2 =
        axis.get_y() * axis.get_z() * (1 - cosine) + axis.get_x() * sine;
    auto value_3_3 = axis.get_z() * axis.get_z() * (1 - cosine) + cosine;
    return matrix_type<3>{value_1_1, value_1_2, value_1_3, value_2_1, value_2_2,
                          value_2_3, value_3_1, value_3_2, value_3_3};
  }

private:
  std::double_t value_1_1;
  std::double_t value_1_2;
  std::double_t value_1_3;
  std::double_t value_2_1;
  std::double_t value_2_2;
  std::double_t value_2_3;
  std::double_t value_3_1;
  std::double_t value_3_2;
  std::double_t value_3_3;
};

#endif
