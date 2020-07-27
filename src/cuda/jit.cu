#include <cuda.h>

#include <misaki/utils/cuda/common.cuh>
#include <sstream>

namespace misaki {
namespace cuda {

char* msk_cuda_whos() {
  std::ostringstream oss;
  oss << std::endl;
  oss << "=========================================" << std::endl;
  return strdup(oss.str().c_str());
}

}  // namespace cuda
}  // namespace misaki
