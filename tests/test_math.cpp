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

int main() {
  test_vector();
  test_color();
  test_frame();
}