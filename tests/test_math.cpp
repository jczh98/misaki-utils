#include <misaki/utils/math.h>

#include <chrono>
#include <iostream>

using namespace misaki;

void test_vector() {
  math::TVector<float, 3> i1(1, 2, 3);
  math::TVector<int, 3> f1(i1);
  std::cout << f1 << std::endl;

  Eigen::Vector3f vv1(1, 1, 1);
  Eigen::Vector3f vv2(1, 2, 3);
  auto now = std::chrono::system_clock::now();
  auto rr1 = vv1.cwiseProduct(vv2);
  auto end = std::chrono::system_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - now).count() << std::endl;

  math::TVector<float, 3> v1(1);
  math::TVector<float, 3> v2(1, 2, 3);

  now = std::chrono::system_clock::now();
  auto r1 = v1 * v2;
  end = std::chrono::system_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - now).count() << std::endl;
  auto r2 = 1 + v1;
  auto r3 = 1 - v1;
  auto r4 = 1 * v1;
  auto r5 = 1 / v1;
  auto r6 = v1 * v2;
  auto r7 = v1 / v2;
  std::cout << r1 << std::endl;
  std::cout << r2 << std::endl;
  std::cout << r3 << std::endl;
  std::cout << r4 << std::endl;
  std::cout << r5 << std::endl;
  std::cout << r6 << std::endl;
  std::cout << r7 << std::endl;

  std::cout << math::clamp(v2, 1.5, 2.5) << std::endl;
  math::TVector<float, 3> vn(1.5, 2.5, 3.5);
  std::cout << math::floor2int(vn) << std::endl;
}

void test_tensor() {
  using namespace misaki;
  math::Tensor<float, 2> t2(math::TVector<int, 2>(2, 2));
  auto t1 = math::Tensor<float, 2>::from_linear_indexed({2, 2}, [&](int i) {
    return i * 1.5;
  });
  std::cout << t1.raw_data()[3] << std::endl;
}

int main() {
  test_vector();
  return 0;
}
