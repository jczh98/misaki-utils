#include <malloc.h>

#include <misaki/utils/stl/memory_resource.hpp>

namespace mstd {

namespace pmr {

memory_resource::~memory_resource() {}

class NewDeleteResource : public memory_resource {
  void *do_allocate(size_t size, size_t alignment) {
#if defined(MSK_HAVE__ALIGNED_MALLOC)
    return _aligned_malloc(size, alignment);
#elif defined(MSK_HAVE_POSIX_MEMALIGN)
    void *ptr;
    if (alignment < sizeof(void *))
      return malloc(size);
    if (posix_memalign(&ptr, alignment, size) != 0)
      ptr = nullptr;
    return ptr;
#else
    return _aligned_malloc(alignment, size);
#endif
  }

  void do_deallocate(void *ptr, size_t bytes, size_t alignment) {
    if (ptr == nullptr)
      return;
#if defined(MSK_HAVE__ALIGNED_MALLOC)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
  }

  bool do_is_equal(const memory_resource &other) const noexcept {
    return this == &other;
  }
};

static NewDeleteResource ndr;

memory_resource *new_delete_resource() noexcept {
  return &ndr;
}

static memory_resource *defaultMemoryResource = new_delete_resource();

memory_resource *set_default_resource(memory_resource *r) noexcept {
  memory_resource *orig = defaultMemoryResource;
  defaultMemoryResource = r;
  return orig;
}

memory_resource *get_default_resource() noexcept {
  return defaultMemoryResource;
}

}  // namespace pmr
}  // namespace mstd