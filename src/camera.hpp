#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "SDL3/SDL_keycode.h"

#include "configuration.hpp"
#include "matrix.hpp"
#include "vector.hpp"

#include <cmath>

class camera_type {
public:
  [[nodiscard]] auto get_focal_length() const -> std::double_t;
  [[nodiscard]] auto get_position() const -> vector_type;
  [[nodiscard]] auto get_direction() const -> vector_type;
  auto set_focal_length(std::double_t) -> void;
  auto set_position(vector_type position) -> void;
  auto set_direction(vector_type direction) -> void;
  auto set_input_state(std::float_t x_axis_change, std::float_t y_axis_change)
      -> void;
  auto set_input_state(SDL_Keycode keycode, bool state) -> void;
  auto rotate() -> void;
  auto move() -> void;
  auto compute_matrices() -> void;
  [[nodiscard]] auto get_change_of_basis_matrix() const -> matrix_type<3>;
  [[nodiscard]] auto get_inverse_change_of_basis_matrix() const
      -> matrix_type<3>;

private:
  std::double_t focal_length;
  vector_type position;
  vector_type direction;
  vector_type velocity;
  matrix_type<3> change_of_basis_matrix;
  matrix_type<3> inverse_change_of_basis_matrix;
  struct {
    bool rotate_up;
    bool rotate_down;
    bool rotate_left;
    bool rotate_right;
    bool move_upward;
    bool move_downward;
    bool move_leftward;
    bool move_rightward;
    bool move_forward;
    bool move_backward;
  } input_state;
};

#endif
