#include <iostream>
#include <misaki/utils/cuda.hpp>

using namespace misaki::cuda;

int main() {
  std::cout << msk_cuda_whos() << std::endl;
  return 0;
}