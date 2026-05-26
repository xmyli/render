#ifndef MESH_HPP
#define MESH_HPP

#include <cmath>
#include <functional>
#include <tuple>
#include <vector>

#include "camera.hpp"
#include "face.hpp"
#include "vector.hpp"

class mesh_type {
public:
  auto add_face(vector_type vertex1, vector_type vertex2, vector_type vertex3,
                std::double_t red, std::double_t green, std::double_t blue)
      -> void {
    auto vertex1_index = this->vertices.size(); // TODO: check if already exists
    this->vertices.push_back(vertex1);
    auto vertex2_index = this->vertices.size(); // TODO: check if already exists
    this->vertices.push_back(vertex2);
    auto vertex3_index = this->vertices.size(); // TODO: check if already exists
    this->vertices.push_back(vertex3);
    auto color = color_type{red, green, blue};
    this->faces.emplace_back(vertex1_index, vertex2_index, vertex3_index,
                             color);
    this->normals.push_back(
        (vertex3 - vertex1).cross_product(vertex2 - vertex1).get_normalized());
  }

  [[nodiscard]] auto get_number_of_faces() const -> std::size_t {
    return this->faces.size();
  }

  [[nodiscard]] auto get_face_by_index(std::size_t index,
                                       const camera_type &camera) const
      -> face_type {
    auto vertex_1 = this->vertices.at(std::get<0>(this->faces.at(index)));
    auto vertex_2 = this->vertices.at(std::get<1>(this->faces.at(index)));
    auto vertex_3 = this->vertices.at(std::get<2>(this->faces.at(index)));
    auto color = std::get<color_type>(this->faces.at(index));
    auto normal_vector = this->normals.at(index);
    return {vertex_1, vertex_2, vertex_3, color, normal_vector, camera};
  }

  auto get_vertex_by_index(std::size_t index) -> vector_type {
    return this->vertices.at(index);
  }

  auto transform_vertices(const std::function<vector_type(vector_type)> &func)
      -> void {
    for (std::size_t vertex_index = 0; vertex_index < this->vertices.size();
         vertex_index += 1) {
      this->vertices.at(vertex_index) = func(this->vertices.at(vertex_index));
    }
  }

private:
  std::vector<vector_type> vertices;
  std::vector<std::tuple<std::size_t, std::size_t, std::size_t, color_type>>
      faces;
  std::vector<vector_type> normals;
};

#endif
