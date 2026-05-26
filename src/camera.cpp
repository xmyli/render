#include "camera.hpp"
#include "configuration.hpp"
#include "matrix.hpp"
#include <iostream>

[[nodiscard]] auto camera_type::get_focal_length() const -> std::double_t {
  return this->focal_length;
}

[[nodiscard]] auto camera_type::get_position() const -> vector_type {
  return this->position;
}

[[nodiscard]] auto camera_type::get_direction() const -> vector_type {
  return this->direction;
}

auto camera_type::set_focal_length(std::double_t focal_length) -> void {
  this->focal_length = focal_length;
}

auto camera_type::set_position(vector_type position) -> void {
  this->position = position;
}

auto camera_type::set_direction(vector_type direction) -> void {
  this->direction = direction.get_normalized();
}

auto camera_type::set_input_state(std::float_t x_axis_change,
                                  std::float_t y_axis_change) -> void {
  if (x_axis_change > 0) {
    this->input_state.rotate_right = true;
  }
  if (x_axis_change < 0) {
    this->input_state.rotate_left = true;
  }
  if (y_axis_change > 0) {
    this->input_state.rotate_down = true;
  }
  if (y_axis_change < 0) {
    this->input_state.rotate_up = true;
  }
}

auto camera_type::set_input_state(SDL_Keycode keycode, bool state) -> void {
  if (keycode == SDLK_SPACE) {
    this->input_state.move_upward = state;
  }
  if (keycode == SDLK_LSHIFT || keycode == SDLK_RSHIFT) {
    this->input_state.move_downward = state;
  }
  if (keycode == SDLK_W) {
    this->input_state.move_forward = state;
  }
  if (keycode == SDLK_A) {
    this->input_state.move_leftward = state;
  }
  if (keycode == SDLK_S) {
    this->input_state.move_backward = state;
  }
  if (keycode == SDLK_D) {
    this->input_state.move_rightward = state;
  }
}

auto camera_type::rotate() -> void {
  if (this->input_state.rotate_right) {
    auto rotation_matrix = matrix_type<3>::get_rotation_matrix(
        camera_rotation_speed, vector_type{0, 1, 0});
    this->direction = rotation_matrix * this->direction;
    this->direction = this->direction.get_normalized();
    this->input_state.rotate_right = false;
  }
  if (this->input_state.rotate_left) {
    auto rotation_matrix = matrix_type<3>::get_rotation_matrix(
        -camera_rotation_speed, vector_type{0, 1, 0});
    this->direction = rotation_matrix * this->direction;
    this->direction = this->direction.get_normalized();
    this->input_state.rotate_left = false;
  }
  if (this->input_state.rotate_down) {
    if (std::asin(this->direction.get_normalized().get_y()) *
                radian_to_degree_conversion_factor +
            camera_rotation_speed >=
        90) {
      this->input_state.rotate_down = false;
      return;
    }
    auto rotation_matrix = matrix_type<3>::get_rotation_matrix(
        camera_rotation_speed,
        this->direction.cross_product(vector_type{0, 1, 0}));
    this->direction = rotation_matrix * this->direction;
    this->direction = this->direction.get_normalized();
    this->input_state.rotate_down = false;
  }
  if (this->input_state.rotate_up) {
    if (std::asin(this->direction.get_normalized().get_y()) *
                radian_to_degree_conversion_factor -
            camera_rotation_speed <=
        -90) {
      this->input_state.rotate_up = false;
      return;
    }
    auto rotation_matrix = matrix_type<3>::get_rotation_matrix(
        -camera_rotation_speed,
        this->direction.cross_product(vector_type{0, 1, 0}));
    this->direction = rotation_matrix * this->direction;
    this->direction = this->direction.get_normalized();
    this->input_state.rotate_up = false;
  }
}

auto camera_type::move() -> void {
  auto velocity_change = vector_type{};
  if (this->input_state.move_upward) {
    auto upward_vector = vector_type{0, -1, 0}.get_normalized();
    velocity_change = velocity_change + upward_vector;
  }
  if (this->input_state.move_downward) {
    auto downward_vector = vector_type{0, 1, 0}.get_normalized();
    velocity_change = velocity_change + downward_vector;
  }
  if (this->input_state.move_forward) {
    auto forward_vector =
        vector_type{this->direction.get_x(), 0, this->direction.get_z()}
            .get_normalized();
    velocity_change = velocity_change + forward_vector;
  }
  if (this->input_state.move_leftward) {
    auto leftward_vector =
        vector_type{-this->direction.get_z(), 0, this->direction.get_x()}
            .get_normalized();
    velocity_change = velocity_change + leftward_vector;
  }
  if (this->input_state.move_backward) {
    auto backward_vector =
        vector_type{-this->direction.get_x(), 0, -this->direction.get_z()}
            .get_normalized();
    velocity_change = velocity_change + backward_vector;
  }
  if (this->input_state.move_rightward) {
    auto rightward_vector =
        vector_type{this->direction.get_z(), 0, -this->direction.get_x()}
            .get_normalized();
    velocity_change = velocity_change + rightward_vector;
  }
  if (velocity_change.get_length() > 0) {
    velocity_change = velocity_change.get_normalized();
    this->velocity = (this->velocity + velocity_change).get_normalized() *
                     camera_movement_speed;
  }
  this->position = this->position + this->velocity;
  this->velocity = this->velocity * (1.0 / 2);
}

auto camera_type::compute_matrices() -> void {
  auto x_axis = this->direction.get_normalized()
                    .cross_product({0, -1, 0})
                    .get_normalized();
  auto y_axis =
      this->direction.get_normalized().cross_product(x_axis).get_normalized();
  auto z_axis = this->direction.get_normalized();
  auto value_1_1 = x_axis.get_x();
  auto value_1_2 = y_axis.get_x();
  auto value_1_3 = z_axis.get_x();
  auto value_2_1 = x_axis.get_y();
  auto value_2_2 = y_axis.get_y();
  auto value_2_3 = z_axis.get_y();
  auto value_3_1 = x_axis.get_z();
  auto value_3_2 = y_axis.get_z();
  auto value_3_3 = z_axis.get_z();
  this->change_of_basis_matrix = {value_1_1, value_1_2, value_1_3,
                                  value_2_1, value_2_2, value_2_3,
                                  value_3_1, value_3_2, value_3_3};
  auto inverse_change_of_basis_matrix =
      this->change_of_basis_matrix.get_inversion();
  if (inverse_change_of_basis_matrix.has_value()) {
    this->inverse_change_of_basis_matrix =
        inverse_change_of_basis_matrix.value();
  }
}

auto camera_type::get_change_of_basis_matrix() const -> matrix_type<3> {
  return this->change_of_basis_matrix;
}

auto camera_type::get_inverse_change_of_basis_matrix() const -> matrix_type<3> {
  return this->inverse_change_of_basis_matrix;
}
