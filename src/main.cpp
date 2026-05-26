#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

#include "camera.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "testing.hpp"

#include <array>
#include <cstddef>
#include <iostream>
#include <numbers>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static camera_type camera;
static mesh_type object;
static std::vector<std::vector<fragment_type>> fragment_buffer;
static SDL_Texture *texture;
static std::array<uint32_t, width * height> texture_data;
static std::uint64_t number_of_frames_elapsed = 0;
static std::uint64_t previous_time = 0;
static std::vector<light_type> lights;

auto SDL_AppInit(void ** /*appstate*/, int /*argc*/, char * /*argv*/[])
    -> SDL_AppResult {
  if (!SDL_CreateWindowAndRenderer("Renderer", width, height, 0, &window,
                                   &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STREAMING, width, height);

  fragment_buffer.resize(width);
  for (auto &inner : fragment_buffer) {
    inner.resize(height);
  }

  camera.set_focal_length(1);
  camera.set_position({0.5, -0.5, -0.5});
  camera.set_direction({-1, 1, 1});

  lights.push_back({{-0.75, -0.75, 0.75}, {0.5, 0.5, 1.0}, 0.05});

  object = generate_test_cube(0.5);

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
auto SDL_AppEvent(void *appstate, SDL_Event *event) -> SDL_AppResult {
  if (event->type == SDL_EVENT_MOUSE_MOTION) {
    camera.set_input_state(event->motion.xrel, event->motion.yrel);
  }
  if (event->type == SDL_EVENT_KEY_DOWN) {
    camera.set_input_state(event->key.key, true);
  }
  if (event->type == SDL_EVENT_KEY_UP) {
    camera.set_input_state(event->key.key, false);
  }
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
auto SDL_AppIterate(void *appstate) -> SDL_AppResult {
  // Compute screen details
  auto time = SDL_GetTicks();
  if (time > previous_time + 1000) {
    std::cout << "FPS: " << number_of_frames_elapsed << "\n";
    number_of_frames_elapsed = 1;
    previous_time = time;
  } else {
    number_of_frames_elapsed += 1;
  }

  camera.rotate();
  camera.move();
  camera.compute_matrices();

  lights.at(0).position =
      matrix_type<3>::get_rotation_matrix(2, {0, 1, 0}) * lights.at(0).position;

  // Vertex Processing
  auto mesh_copy = object;
  mesh_copy.transform_vertices([](vector_type vertex) {
    auto translated_vertex = vertex - camera.get_position();
    auto matrix = camera.get_inverse_change_of_basis_matrix();
    auto rotated_vertex = matrix * translated_vertex;
    auto projected_vertex =
        vector_type{camera.get_focal_length() * rotated_vertex.get_x() /
                        rotated_vertex.get_z(),
                    camera.get_focal_length() * rotated_vertex.get_y() /
                        rotated_vertex.get_z(),
                    rotated_vertex.get_z()};
    return projected_vertex;
  });

  // Rasterization
  for (std::size_t face_index = 0; face_index < mesh_copy.get_number_of_faces();
       face_index += 1) {
    auto face = mesh_copy.get_face_by_index(face_index, camera);
    if (!face.is_visible()) {
      continue;
    }

    auto minimum_x_index = std::max(
        static_cast<int>((face.get_minimum_x() + 1) / fragment_width), 0);
    auto maximum_x_index = std::min(
        static_cast<int>((face.get_maximum_x() + 1) / fragment_width) + 1,
        width);
    auto minimum_y_index = std::max(
        static_cast<int>((face.get_minimum_y() + 1) / fragment_height), 0);
    auto maximum_y_index = std::min(
        static_cast<int>((face.get_maximum_y() + 1) / fragment_height) + 1,
        height);

    std::double_t fragment_camera_space_x_coordinate =
        minimum_x_index * fragment_width - 1;
    std::double_t fragment_camera_space_y_coordinate =
        minimum_y_index * fragment_height - 1;

    for (auto fragment_buffer_x_index = minimum_x_index;
         fragment_buffer_x_index < maximum_x_index;
         fragment_buffer_x_index += 1) {
      for (auto fragment_buffer_y_index = minimum_y_index;
           fragment_buffer_y_index < maximum_y_index;
           fragment_buffer_y_index += 1) {
        auto fragment =
            face.get_fragment({fragment_camera_space_x_coordinate,
                               fragment_camera_space_y_coordinate, 0});
        if (fragment.has_value()) {
          auto &buffered_fragment = fragment_buffer.at(fragment_buffer_x_index)
                                        .at(fragment_buffer_y_index);
          if (buffered_fragment.z_depth == -1 ||
              buffered_fragment.z_depth > fragment.value().z_depth) {
            buffered_fragment = fragment.value();
          }
        }
        fragment_camera_space_y_coordinate += fragment_height;
      }
      fragment_camera_space_x_coordinate += fragment_width;
      fragment_camera_space_y_coordinate =
          minimum_y_index * fragment_height - 1;
    }
  }

  auto change_of_basis_matrix = camera.get_change_of_basis_matrix();
  auto inverse_change_of_basis_matrix =
      camera.get_inverse_change_of_basis_matrix();

  std::double_t fragment_camera_space_x_coordinate = -1;
  std::double_t fragment_camera_space_y_coordinate = -1;
  for (auto fragment_buffer_x_index = 0; fragment_buffer_x_index < width;
       fragment_buffer_x_index += 1) {
    for (auto fragment_buffer_y_index = 0; fragment_buffer_y_index < height;
         fragment_buffer_y_index += 1) {
      auto &fragment = fragment_buffer.at(fragment_buffer_x_index)
                           .at(fragment_buffer_y_index);
      auto ray_in_camera_space = vector_type(fragment_camera_space_x_coordinate,
                                             fragment_camera_space_y_coordinate,
                                             camera.get_focal_length());
      fragment.ray =
          (change_of_basis_matrix * ray_in_camera_space).get_normalized();
      if (fragment.z_depth == -1) {
        fragment.z_depth = 0;
        fragment.color = color_type{0.25, 0.5, 0.75};
      }
      fragment_camera_space_y_coordinate += fragment_height;
    }
    fragment_camera_space_x_coordinate += fragment_width;
    fragment_camera_space_y_coordinate = -1;
  }

  // Fragment shading: compute ray vectors and set background
  /*
  for (std::double_t x_coordinate = -1; x_coordinate < 1;
       x_coordinate += 1.0 / width) {
    auto x_fragment_index = static_cast<int>(width / 2.0 * (x_coordinate + 1));
    for (std::double_t y_coordinate = -1; y_coordinate < 1;
         y_coordinate += 1.0 / height) {
      auto y_fragment_index =
          static_cast<int>(height / 2.0 * (y_coordinate + 1));
      auto &fragment =
          fragment_buffer.at(x_fragment_index).at(y_fragment_index);
      auto ray_in_camera_space =
          vector_type(x_coordinate, y_coordinate, camera.get_focal_length());
      fragment.ray =
          (change_of_basis_matrix * ray_in_camera_space).get_normalized();
      if (fragment.z_depth == -1) {
        fragment.z_depth = 0;
        fragment.color = color_type{0.25, 0.5, 0.75};
      }
    }
  }
*/

  // Fragment shading: do lighting
  for (auto &light : lights) {
    for (auto x_coordinate = 0; x_coordinate < width; x_coordinate += 1) {
      for (auto y_coordinate = 0; y_coordinate < height; y_coordinate += 1) {
        auto &fragment = fragment_buffer.at(x_coordinate).at(y_coordinate);
        if (fragment.z_depth > 0) {
          auto view_direction = fragment.ray;
          auto light_direction =
              (fragment.position - light.position).get_normalized();
          auto halfway_vector =
              (view_direction + light_direction).get_normalized();

          auto distance_to_light =
              (light.position - fragment.position).get_length();

          auto specular_coefficient = std::pow(
              std::max(halfway_vector.dot_product(fragment.normal) * -1, 0.0),
              specular_exponent * std::pow(distance_to_light, 3));

          auto diffuse_coefficient =
              std::max(light_direction.dot_product(fragment.normal) * -1, 0.0) /
              std::pow(distance_to_light, 3);

          auto ambience_coefficient = 0.25;

          fragment.color.red =
              std::max(std::min(specular_coefficient * light.color.red +
                                    diffuse_coefficient * fragment.color.red +
                                    ambience_coefficient * fragment.color.red,
                                1.0),
                       0.0);
          fragment.color.green =
              std::max(std::min(specular_coefficient * light.color.green +
                                    diffuse_coefficient * fragment.color.green +
                                    ambience_coefficient * fragment.color.green,
                                1.0),
                       0.0);
          fragment.color.blue =
              std::max(std::min(specular_coefficient * light.color.blue +
                                    diffuse_coefficient * fragment.color.blue +
                                    ambience_coefficient * fragment.color.blue,
                                1.0),
                       0.0);
        }
      }
    }
  }

  // fragment shading: render lights
  for (auto &light : lights) {
    auto light_depth =
        (inverse_change_of_basis_matrix * light.position).get_z();
    auto camera_to_light_vector = light.position - camera.get_position();
    auto attenuation_factor =
        8 * std::pow(camera_to_light_vector.get_length(), 3);
    for (auto x_coordinate = 0; x_coordinate < width; x_coordinate += 1) {
      for (auto y_coordinate = 0; y_coordinate < height; y_coordinate += 1) {
        auto &fragment = fragment_buffer.at(x_coordinate).at(y_coordinate);
        if (fragment.z_depth > 0 && fragment.z_depth < light_depth) {
          continue;
        }
        auto intensity = std::pow(
            fragment.ray.dot_product(camera_to_light_vector.get_normalized()),
            attenuation_factor);
        if (intensity > 0) {
          fragment.color.red =
              std::min(fragment.color.red + intensity * light.color.red, 1.0);
          fragment.color.green = std::min(
              fragment.color.green + intensity * light.color.green, 1.0);
          fragment.color.blue =
              std::min(fragment.color.blue + intensity * light.color.blue, 1.0);
        }
      }
    }
  }

  // Update texture using fragment buffer
  for (std::size_t x_coordinate = 0; x_coordinate < fragment_buffer.size();
       x_coordinate += 1) {
    for (std::size_t y_coordinate = 0;
         y_coordinate < fragment_buffer.at(x_coordinate).size();
         y_coordinate += 1) {
      auto &fragment = fragment_buffer.at(x_coordinate).at(y_coordinate);
      if (fragment.z_depth != -1) {
        fragment.z_depth = -1;
        texture_data.at(width * y_coordinate + x_coordinate) =
            (static_cast<uint8_t>(255 * fragment.color.red) << 24) |
            (static_cast<uint8_t>(255 * fragment.color.green) << 16) |
            (static_cast<uint8_t>(255 * fragment.color.blue) << 8) | 0xff;
      } else {
        texture_data.at(width * y_coordinate + x_coordinate) = 0;
      }
    }
  }
  SDL_UpdateTexture(texture, nullptr, &texture_data, width * sizeof(uint32_t));

  // Render texture
  SDL_RenderTexture(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
