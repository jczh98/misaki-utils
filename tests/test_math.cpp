#include <Eigen/Dense>
#include <chrono>
#include <misaki/utils/math.hpp>

using namespace misaki::math;

void test_vector() {
  Vector2f vec2f = {1, 2};
  std::cout << vec2f << std::endl;
  Vector3f vec3f = {1, 2, 3.5};
  std::cout << vec3f << std::endl;
  Vector3i vec3i = {3, 5, 7};
  std::cout << vec3i << std::endl;
  auto conv = Vector3i(vec3f);
  std::cout << conv << std::endl;
  // Unary test
  vec3f -= vec3i;
  std::cout << vec3f;
  vec3f += 1.f;
  std::cout << vec3f;
  vec3f += 2;
  std::cout << vec3f << std::endl;
  std::cout << -vec3f << std::endl;
  // Binary test
  Vector3f rhs = {0.5, 0.6, 0.7};
  auto t1 = vec3f + rhs;
  auto t2 = vec3f - rhs;
  auto t3 = vec3f * rhs;
  auto t4 = vec3f / rhs;
  std::cout << t1 << t2 << t3 << t4 << std::endl;
  auto t5 = vec3f + 1;
  auto t6 = vec3f - 2.f;
  auto t7 = vec3f * 3.;
  auto t8 = vec3f / 5u;
  std::cout << t5 << t6 << t7 << t8 << std::endl;
  // Math function
  std::cout << vec3f.hprod() << " " << vec3f.norm() << " " << vec3f.normalize() << vec3i.clamp(4, 6) << std::endl;
  std::cout << dot(vec3f, rhs) << std::endl;
  std::cout << cross(vec3f, rhs) << std::endl;
  std::cout << vec3f[2] << std::endl;
  std::cout << vec3f.min_coeff() << " " << vec3f.max_coeff() << std::endl;
}

void test_color() {
  Color3f col1 = {0.5, 0.5, 0.5};
  Color4f col2 = {1.0, 0.7, 0.8, 0.5};
  std::cout << col1.is_valid() << " " << col1 << std::endl;
  std::cout << col2.is_valid() << " " << col2 << " " << col2.divide_by_alpha() << std::endl;
}

void test_frame() {
  Frame3f frame({0, 0, 1});
  std::cout << frame << std::endl;
}

void test_matrix() {
  // ----------------------------------------- Matrix4 ------------------------------------
  // Matrix-matrix multiplication
  Eigen::Matrix4f em1;
  Eigen::Matrix4f em2;
  Eigen::Matrix4f em0 = Eigen::Matrix4f::Zero();
  em1 << -0.997497, 0.170019, 0.64568, 0.421003,
      0.127171, -0.0402539, 0.49321, 0.0270699,
      -0.613392, -0.299417, -0.651784, -0.39201,
      0.617481, 0.791925, 0.717887, -0.970031;
  em2 << -0.817194, 0.97705, -0.982177, 0.203528,
      -0.271096, -0.108615, -0.24424, 0.214332,
      -0.705374, -0.761834, 0.0633259, -0.667531,
      -0.668203, -0.990661, 0.142369, 0.32609;
  auto st = std::chrono::system_clock::now();
  for (int i = 0; i < 1000000; i++) {
    em0 += em1 * em2;
  }
  auto ed = std::chrono::system_clock::now();
  std::cout << em0 << " " << std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count() << std::endl;
  Matrix4f m0 = Matrix4f::zeros();
  Matrix4f m1 = Matrix4f(-0.997497, 0.170019, 0.64568, 0.421003,
                         0.127171, -0.0402539, 0.49321, 0.0270699,
                         -0.613392, -0.299417, -0.651784, -0.39201,
                         0.617481, 0.791925, 0.717887, -0.970031);
  Matrix4f m2 = Matrix4f(-0.817194, 0.97705, -0.982177, 0.203528,
                         -0.271096, -0.108615, -0.24424, 0.214332,
                         -0.705374, -0.761834, 0.0633259, -0.667531,
                         -0.668203, -0.990661, 0.142369, 0.32609);
  st = std::chrono::system_clock::now();
  for (int i = 0; i < 1000000; i++) {
    m0 += m1 * m2;
  }
  ed = std::chrono::system_clock::now();
  std::cout << m0 << " " << std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count() << std::endl;
  // Matrix-vector multiplication
  auto evec2 = Eigen::Vector4f(2, 3, 5, 6);
  st = std::chrono::system_clock::now();
  auto evec0 = em1 * evec2;
  auto evec00 = evec2.transpose() * em1;
  ed = std::chrono::system_clock::now();
  std::cout << evec0 << " " << evec00 << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(ed - st).count() << std::endl;
  auto vec2 = Vector4f(2, 3, 5, 6);
  st = std::chrono::system_clock::now();
  auto vec0 = m1 * vec2;
  ed = std::chrono::system_clock::now();
  std::cout << vec0 << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(ed - st).count() << std::endl;
  // Matrix-scalar operation
  std::cout << m1 + 1.f << std::endl;
  std::cout << m1 * 3.f << std::endl;
  auto m11 = m1;
  m11 *= 5.f;
  std::cout << m11 << std::endl;
  // Inverse
  std::cout << em1.inverse() << std::endl;
  std::cout << m1.inverse() << std::endl;
  std::cout << em1.determinant() << " " << m1.determinant() << std::endl;
  std::cout << em1.transpose() << std::endl;
  std::cout << m1.transpose() << std::endl;
  // -------------------------------------------- Matrix3 -----------------------------------------------
  Matrix3f mat1 = Matrix3f(-0.997497, 0.170019, 0.64568,
                           0.127171, -0.0402539, 0.49321,
                           -0.613392, -0.299417, -0.651784);
  Eigen::Matrix3f emat1;
  emat1 << -0.997497, 0.170019, 0.64568,
      0.127171, -0.0402539, 0.49321,
      -0.613392, -0.299417, -0.651784;
  std::cout << emat1.determinant() << " " << mat1.determinant() << std::endl;
  std::cout << emat1.inverse() << std::endl;
  std::cout << mat1.inverse() << std::endl;
}

void test_transform() {
  Matrix4f m1 = Matrix4f(-0.997497, 0.170019, 0.64568, 0.421003,
                         0.127171, -0.0402539, 0.49321, 0.0270699,
                         -0.613392, -0.299417, -0.651784, -0.39201,
                         0.617481, 0.791925, 0.717887, -0.970031);
  Transform4f trans(m1);
  std::cout << trans << std::endl;
  const auto ret = trans.apply_normal({1.f, 2.f, 3.f});
  std::cout << ret << std::endl;
  const auto translate = Transform4f::translate({1, 2, 3});
  const auto rotate = Transform4f::rotate({0, 0, 1}, 90);
  const auto scale = Transform4f::scale({2, 2, 2});
  const auto vec = Vector3f(1, 2, 3);
  std::cout << translate.apply_point(vec) << std::endl;
  std::cout << scale.apply_vector(vec) << std::endl;
  std::cout << rotate.apply_vector(vec) << std::endl;
  std::cout << Transform4f::look_at({0, 0, 1}, {0, 0, 0}, {0, 1, 0}) << std::endl;
  std::cout << Transform4f::perspective(30, 0.1, 10000) << std::endl;
  std::cout << Transform3f::translate({1, 2}).apply_point({2, 1}) << std::endl;
}
int main() {
  // test_vector();
  // test_color();
  // test_frame();
  // test_matrix();
  test_transform();
}