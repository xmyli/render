#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include "mesh.hpp"

#include <cmath>

inline auto generate_test_cube(std::double_t size) -> mesh_type {
  mesh_type mesh;

  auto side_1_face_1_vertex_1 = vector_type{-size / 2, -size / 2, -size / 2};
  auto side_1_face_1_vertex_2 = vector_type{size / 2, size / 2, -size / 2};
  auto side_1_face_1_vertex_3 = vector_type{-size / 2, size / 2, -size / 2};
  auto side_1_face_2_vertex_1 = vector_type{size / 2, size / 2, -size / 2};
  auto side_1_face_2_vertex_2 = vector_type{-size / 2, -size / 2, -size / 2};
  auto side_1_face_2_vertex_3 = vector_type{size / 2, -size / 2, -size / 2};
  mesh.add_face(side_1_face_1_vertex_1, side_1_face_1_vertex_2,
                side_1_face_1_vertex_3, 1.0, 0, 0);
  mesh.add_face(side_1_face_2_vertex_1, side_1_face_2_vertex_2,
                side_1_face_2_vertex_3, 1.0, 0, 0);

  auto side_2_matrix =
      matrix_type<3>::get_rotation_matrix(90, vector_type{0, 1, 0});
  auto side_2_face_1_vertex_1 = side_2_matrix * side_1_face_1_vertex_1;
  auto side_2_face_1_vertex_2 = side_2_matrix * side_1_face_1_vertex_2;
  auto side_2_face_1_vertex_3 = side_2_matrix * side_1_face_1_vertex_3;
  auto side_2_face_2_vertex_1 = side_2_matrix * side_1_face_2_vertex_1;
  auto side_2_face_2_vertex_2 = side_2_matrix * side_1_face_2_vertex_2;
  auto side_2_face_2_vertex_3 = side_2_matrix * side_1_face_2_vertex_3;
  mesh.add_face(side_2_face_1_vertex_1, side_2_face_1_vertex_2,
                side_2_face_1_vertex_3, 0, 1.0, 0);
  mesh.add_face(side_2_face_2_vertex_1, side_2_face_2_vertex_2,
                side_2_face_2_vertex_3, 0, 1.0, 0);

  auto side_3_matrix =
      matrix_type<3>::get_rotation_matrix(180, vector_type{0, 1, 0});
  auto side_3_face_1_vertex_1 = side_3_matrix * side_1_face_1_vertex_1;
  auto side_3_face_1_vertex_2 = side_3_matrix * side_1_face_1_vertex_2;
  auto side_3_face_1_vertex_3 = side_3_matrix * side_1_face_1_vertex_3;
  auto side_3_face_2_vertex_1 = side_3_matrix * side_1_face_2_vertex_1;
  auto side_3_face_2_vertex_2 = side_3_matrix * side_1_face_2_vertex_2;
  auto side_3_face_2_vertex_3 = side_3_matrix * side_1_face_2_vertex_3;
  mesh.add_face(side_3_face_1_vertex_1, side_3_face_1_vertex_2,
                side_3_face_1_vertex_3, 1.0, 0, 0);
  mesh.add_face(side_3_face_2_vertex_1, side_3_face_2_vertex_2,
                side_3_face_2_vertex_3, 1.0, 0, 0);

  auto side_4_matrix =
      matrix_type<3>::get_rotation_matrix(270, vector_type{0, 1, 0});
  auto side_4_face_1_vertex_1 = side_4_matrix * side_1_face_1_vertex_1;
  auto side_4_face_1_vertex_2 = side_4_matrix * side_1_face_1_vertex_2;
  auto side_4_face_1_vertex_3 = side_4_matrix * side_1_face_1_vertex_3;
  auto side_4_face_2_vertex_1 = side_4_matrix * side_1_face_2_vertex_1;
  auto side_4_face_2_vertex_2 = side_4_matrix * side_1_face_2_vertex_2;
  auto side_4_face_2_vertex_3 = side_4_matrix * side_1_face_2_vertex_3;
  mesh.add_face(side_4_face_1_vertex_1, side_4_face_1_vertex_2,
                side_4_face_1_vertex_3, 0, 1.0, 0);
  mesh.add_face(side_4_face_2_vertex_1, side_4_face_2_vertex_2,
                side_4_face_2_vertex_3, 0, 1.0, 0);

  auto side_5_matrix =
      matrix_type<3>::get_rotation_matrix(90, vector_type{1, 0, 0});
  auto side_5_face_1_vertex_1 = side_5_matrix * side_1_face_1_vertex_1;
  auto side_5_face_1_vertex_2 = side_5_matrix * side_1_face_1_vertex_2;
  auto side_5_face_1_vertex_3 = side_5_matrix * side_1_face_1_vertex_3;
  auto side_5_face_2_vertex_1 = side_5_matrix * side_1_face_2_vertex_1;
  auto side_5_face_2_vertex_2 = side_5_matrix * side_1_face_2_vertex_2;
  auto side_5_face_2_vertex_3 = side_5_matrix * side_1_face_2_vertex_3;
  mesh.add_face(side_5_face_1_vertex_1, side_5_face_1_vertex_2,
                side_5_face_1_vertex_3, 0, 0, 1.0);
  mesh.add_face(side_5_face_2_vertex_1, side_5_face_2_vertex_2,
                side_5_face_2_vertex_3, 0, 0, 1.0);

  auto side_6_matrix =
      matrix_type<3>::get_rotation_matrix(-90, vector_type{1, 0, 0});
  auto side_6_face_1_vertex_1 = side_6_matrix * side_1_face_1_vertex_1;
  auto side_6_face_1_vertex_2 = side_6_matrix * side_1_face_1_vertex_2;
  auto side_6_face_1_vertex_3 = side_6_matrix * side_1_face_1_vertex_3;
  auto side_6_face_2_vertex_1 = side_6_matrix * side_1_face_2_vertex_1;
  auto side_6_face_2_vertex_2 = side_6_matrix * side_1_face_2_vertex_2;
  auto side_6_face_2_vertex_3 = side_6_matrix * side_1_face_2_vertex_3;
  mesh.add_face(side_6_face_1_vertex_1, side_6_face_1_vertex_2,
                side_6_face_1_vertex_3, 0, 0, 1.0);
  mesh.add_face(side_6_face_2_vertex_1, side_6_face_2_vertex_2,
                side_6_face_2_vertex_3, 0, 0, 1.0);

  return mesh;
}

#endif
