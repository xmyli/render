#ifndef FACE_HPP
#define FACE_HPP

#include "camera.hpp"
#include "fragment.hpp"
#include "matrix.hpp"
#include "vector.hpp"

#include <algorithm>
#include <optional>

class face_type {
public:
  face_type(vector_type vertex_1, vector_type vertex_2, vector_type vertex_3,
            color_type color, vector_type normal, const camera_type &camera)
      : vertex_1(vertex_1), vertex_2(vertex_2), vertex_3(vertex_3),
        color(color), normal(normal) {
    auto change_of_basis_matrix = camera.get_change_of_basis_matrix();
    this->original_vertex_1 =
        change_of_basis_matrix * this->vertex_1 + camera.get_position();
    this->original_vertex_2 =
        change_of_basis_matrix * this->vertex_2 + camera.get_position();
    this->original_vertex_3 =
        change_of_basis_matrix * this->vertex_3 + camera.get_position();

    auto vertex_1_x = vertex_1.get_x();
    auto vertex_1_y = vertex_1.get_y();
    auto vertex_2_x = vertex_2.get_x();
    auto vertex_2_y = vertex_2.get_y();
    auto vertex_3_x = vertex_3.get_x();
    auto vertex_3_y = vertex_3.get_y();

    this->total_area = vertex_1_x * vertex_2_y + vertex_1_y * vertex_3_x +
                       vertex_2_x * vertex_3_y - vertex_2_y * vertex_3_x -
                       vertex_1_y * vertex_2_x - vertex_1_x * vertex_3_y;
    this->vertex_1_x_coefficient = (vertex_2_y - vertex_3_y) / total_area;
    this->vertex_1_y_coefficient = (vertex_3_x - vertex_2_x) / total_area;
    this->vertex_1_constant =
        (vertex_2_x * vertex_3_y - vertex_2_y * vertex_3_x) / total_area;

    this->vertex_2_x_coefficient = (vertex_3_y - vertex_1_y) / total_area;
    this->vertex_2_y_coefficient = (vertex_1_x - vertex_3_x) / total_area;
    this->vertex_2_constant =
        (vertex_1_y * vertex_3_x - vertex_1_x * vertex_3_y) / total_area;
  }

  auto get_maximum_x() -> std::double_t {
    return std::max(std::max(this->vertex_1.get_x(), this->vertex_2.get_x()),
                    this->vertex_3.get_x());
  }

  auto get_maximum_y() -> std::double_t {
    return std::max(std::max(this->vertex_1.get_y(), this->vertex_2.get_y()),
                    this->vertex_3.get_y());
  }

  auto get_minimum_x() -> std::double_t {
    return std::min(std::min(this->vertex_1.get_x(), this->vertex_2.get_x()),
                    this->vertex_3.get_x());
  }

  auto get_minimum_y() -> std::double_t {
    return std::min(std::min(this->vertex_1.get_y(), this->vertex_2.get_y()),
                    this->vertex_3.get_y());
  }

  [[nodiscard]] auto is_visible() const -> bool {
    return this->vertex_1.get_z() > 0 && this->vertex_2.get_z() > 0 &&
           this->vertex_3.get_z() > 0 && this->total_area > 0;
  }

  [[nodiscard]] auto get_fragment(const vector_type &vector) const
      -> std::optional<fragment_type> {
    if (this->total_area <= 0) {
      return std::nullopt;
    }

    auto vertex_1_weight = this->vertex_1_x_coefficient * vector.get_x() +
                           this->vertex_1_y_coefficient * vector.get_y() +
                           this->vertex_1_constant;
    auto vertex_2_weight = this->vertex_2_x_coefficient * vector.get_x() +
                           this->vertex_2_y_coefficient * vector.get_y() +
                           this->vertex_2_constant;
    auto vertex_3_weight = 1 - vertex_1_weight - vertex_2_weight;

    if (vertex_1_weight < 0 || vertex_1_weight > 1 || vertex_2_weight < 0 ||
        vertex_2_weight > 1 || vertex_3_weight < 0 || vertex_3_weight > 1) {
      return std::nullopt;
    }

    auto z_depth = vertex_1_weight * this->vertex_1.get_z() +
                   vertex_2_weight * this->vertex_2.get_z() +
                   vertex_3_weight * this->vertex_3.get_z();

    auto position = this->original_vertex_1 * vertex_1_weight +
                    this->original_vertex_2 * vertex_2_weight +
                    this->original_vertex_3 * vertex_3_weight;

    return std::make_optional(
        fragment_type{z_depth, this->color, this->normal, {}, position});
  }

private:
  vector_type original_vertex_1;
  vector_type original_vertex_2;
  vector_type original_vertex_3;

  vector_type vertex_1;
  vector_type vertex_2;
  vector_type vertex_3;
  color_type color;
  vector_type normal;

  std::double_t total_area;
  std::double_t vertex_1_x_coefficient;
  std::double_t vertex_1_y_coefficient;
  std::double_t vertex_1_constant;
  std::double_t vertex_2_x_coefficient;
  std::double_t vertex_2_y_coefficient;
  std::double_t vertex_2_constant;
};

#endif
