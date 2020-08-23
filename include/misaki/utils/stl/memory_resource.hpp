#pragma once

#include "../system.h"
#include "../util.h"

namespace mstd {

namespace pmr {

class memory_resource {
  static constexpr size_t max_align = alignof(std::max_align_t);

 public:
  virtual ~memory_resource();
  void *allocate(size_t bytes, size_t alignment = max_align) {
    return do_allocate(bytes, alignment);
  }
  void deallocate(void *p, size_t bytes, size_t alignment = max_align) {
    return do_deallocate(p, bytes, alignment);
  }
  bool is_equal(const memory_resource &other) const noexcept {
    return do_is_equal(other);
  }

 private:
  virtual void *do_allocate(size_t bytes, size_t alignment) = 0;
  virtual void do_deallocate(void *p, size_t bytes, size_t alignment) = 0;
  virtual bool do_is_equal(const memory_resource &other) const noexcept = 0;
};

inline bool operator==(const memory_resource &a, const memory_resource &b) noexcept {
  return a.is_equal(b);
}

inline bool operator!=(const memory_resource &a, const memory_resource &b) noexcept {
  return !(a == b);
}

// TODO
struct pool_options {
  size_t max_blocks_per_chunk = 0;
  size_t largest_required_pool_block = 0;
};
class synchronized_pool_resource;
class unsynchronized_pool_resource;

// global memory resources
memory_resource *new_delete_resource() noexcept;
// TODO: memory_resource* null_memory_resource() noexcept;
memory_resource *set_default_resource(memory_resource *r) noexcept;
memory_resource *get_default_resource() noexcept;

class monotonic_buffer_resource : public memory_resource {
 public:
  explicit monotonic_buffer_resource(memory_resource *upstream)
      : upstreamResource(upstream) {}
  monotonic_buffer_resource(size_t blockSize, memory_resource *upstream)
      : blockSize(blockSize), upstreamResource(upstream) {}
#if 0
    // TODO
    monotonic_buffer_resource(void *buffer, size_t buffer_size,
                              memory_resource *upstream);
#endif
  monotonic_buffer_resource() : monotonic_buffer_resource(get_default_resource()) {}
  explicit monotonic_buffer_resource(size_t initial_size)
      : monotonic_buffer_resource(initial_size, get_default_resource()) {}
#if 0
    // TODO
    monotonic_buffer_resource(void *buffer, size_t buffer_size)
        : monotonic_buffer_resource(buffer, buffer_size, get_default_resource()) {}
#endif
  monotonic_buffer_resource(const monotonic_buffer_resource &) = delete;

  ~monotonic_buffer_resource() { release(); }

  monotonic_buffer_resource operator=(const monotonic_buffer_resource &) = delete;

  void release() {
    for (const auto &block : usedBlocks)
      upstreamResource->deallocate(block.ptr, block.size);
    usedBlocks.clear();

    for (const auto &block : availableBlocks)
      upstreamResource->deallocate(block.ptr, block.size);
    availableBlocks.clear();

    upstreamResource->deallocate(currentBlock.ptr, currentBlock.size);
    currentBlock = MemoryBlock();
  }

  memory_resource *upstream_resource() const { return upstreamResource; }

 protected:
  void *do_allocate(size_t bytes, size_t align) override {
    if (bytes > blockSize) {
      // We've got a big allocation; let the current block be so that
      // smaller allocations have a chance at using up more of it.
      usedBlocks.push_back(
          MemoryBlock{upstreamResource->allocate(bytes, align), bytes});
      return usedBlocks.back().ptr;
    }

    if ((currentBlockPos % align) != 0)
      currentBlockPos += align - (currentBlockPos % align);
    DCHECK_EQ(0, currentBlockPos % align);

    if (currentBlockPos + bytes > currentBlock.size) {
      // Add current block to _usedBlocks_ list
      if (currentBlock.size) {
        usedBlocks.push_back(currentBlock);
        currentBlock = {};
      }

      // Get new block of memory for _MemoryArena_

      // Try to get memory block from _availableBlocks_
      for (auto iter = availableBlocks.begin(); iter != availableBlocks.end();
           ++iter) {
        if (bytes <= iter->size) {
          currentBlock = std::move(*iter);
          availableBlocks.erase(iter);
          goto success;
        }
      }
      currentBlock = {
          upstreamResource->allocate(blockSize, alignof(std::max_align_t)),
          blockSize};
    success:
      currentBlockPos = 0;
    }

    void *ptr = (char *)currentBlock.ptr + currentBlockPos;
    currentBlockPos += bytes;
    return ptr;
  }

  void do_deallocate(void *p, size_t bytes, size_t alignment) override {
    // no-op
  }

  bool do_is_equal(const memory_resource &other) const noexcept override {
    return this == &other;
  }

 private:
  struct MemoryBlock {
    void *ptr = nullptr;
    size_t size = 0;
  };

  memory_resource *upstreamResource;
  size_t blockSize = 256 * 1024;
  MemoryBlock currentBlock;
  size_t currentBlockPos = 0;
  // TODO: should use the memory_resource for this list's allocations...
  std::list<MemoryBlock> usedBlocks, availableBlocks;
};

template <class Tp = std::byte>
class polymorphic_allocator {
 public:
  using value_type = Tp;

  polymorphic_allocator() noexcept { memoryResource = new_delete_resource(); }
  polymorphic_allocator(memory_resource *r) : memoryResource(r) {}
  polymorphic_allocator(const polymorphic_allocator &other) = default;
  template <class U>
  polymorphic_allocator(const polymorphic_allocator<U> &other) noexcept
      : memoryResource(other.resource()) {}

  polymorphic_allocator &operator=(const polymorphic_allocator &rhs) = delete;

  // member functions
  [[nodiscard]] Tp *allocate(size_t n) {
    return static_cast<Tp *>(resource()->allocate(n * sizeof(Tp), alignof(Tp)));
  }
  void deallocate(Tp *p, size_t n) { resource()->deallocate(p, n); }

  void *allocate_bytes(size_t nbytes, size_t alignment = alignof(max_align_t)) {
    return resource()->allocate(nbytes, alignment);
  }
  void deallocate_bytes(void *p, size_t nbytes,
                        size_t alignment = alignof(std::max_align_t)) {
    return resource()->deallocate(p, nbytes, alignment);
  }
  template <class T>
  T *allocate_object(size_t n = 1) {
    return static_cast<T *>(allocate_bytes(n * sizeof(T), alignof(T)));
  }
  template <class T>
  void deallocate_object(T *p, size_t n = 1) {
    deallocate_bytes(p, n * sizeof(T), alignof(T));
  }
  template <class T, class... Args>
  T *new_object(Args &&... args) {
    // NOTE: this doesn't handle constructors that throw exceptions...
    T *p = allocate_object<T>();
    construct(p, std::forward<Args>(args)...);
    return p;
  }
  template <class T>
  void delete_object(T *p) {
    destroy(p);
    deallocate_object(p);
  }

  template <class T, class... Args>
  void construct(T *p, Args &&... args) {
    ::new ((void *)p) T(std::forward<Args>(args)...);
  }

  template <class T>
  void destroy(T *p) {
    p->~T();
  }

  // polymorphic_allocator select_on_container_copy_construction() const;

  memory_resource *resource() const { return memoryResource; }

 private:
  memory_resource *memoryResource;
};

template <class T1, class T2>
bool operator==(const polymorphic_allocator<T1> &a,
                const polymorphic_allocator<T2> &b) noexcept {
  return a.resource() == b.resource();
}

template <class T1, class T2>
bool operator!=(const polymorphic_allocator<T1> &a,
                const polymorphic_allocator<T2> &b) noexcept {
  return !(a == b);
}

}  // namespace pmr

template <typename T, class Allocator = pmr::polymorphic_allocator<T>>
class vector {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const iterator>;

  vector(const Allocator &alloc = {}) : alloc(alloc) {}
  vector(size_t count, const T &value, const Allocator &alloc = {}) : alloc(alloc) {
    reserve(count);
    for (size_t i = 0; i < count; ++i)
      this->alloc.template construct<T>(ptr + i, value);
    nStored = count;
  }
  vector(size_t count, const Allocator &alloc = {}) : vector(count, T{}, alloc) {}
  vector(const vector &other, const Allocator &alloc = {}) : alloc(alloc) {
    reserve(other.size());
    for (size_t i = 0; i < other.size(); ++i)
      this->alloc.template construct<T>(ptr + i, other[i]);
    nStored = other.size();
  }
  template <class InputIt>
  vector(InputIt first, InputIt last, const Allocator &alloc = {}) : alloc(alloc) {
    reserve(last - first);
    size_t i = 0;
    for (InputIt iter = first; iter != last; ++iter, ++i)
      this->alloc.template construct<T>(ptr + i, *iter);
    nStored = nAlloc;
  }
  vector(vector &&other) : alloc(other.alloc) {
    nStored = other.nStored;
    nAlloc = other.nAlloc;
    ptr = other.ptr;

    other.nStored = other.nAlloc = 0;
    other.ptr = nullptr;
  }
  vector(vector &&other, const Allocator &alloc) {
    if (alloc == other.alloc) {
      ptr = other.ptr;
      nAlloc = other.nAlloc;
      nStored = other.nStored;

      other.ptr = nullptr;
      other.nAlloc = other.nStored = 0;
    } else {
      reserve(other.size());
      for (size_t i = 0; i < other.size(); ++i)
        alloc.template construct<T>(ptr + i, std::move(other[i]));
      nStored = other.size();
    }
  }
  vector(std::initializer_list<T> init, const Allocator &alloc = {})
      : vector(init.begin(), init.end(), alloc) {}

  vector &operator=(const vector &other) {
    if (this == &other)
      return *this;

    clear();
    reserve(other.size());
    for (size_t i = 0; i < other.size(); ++i)
      alloc.template construct<T>(ptr + i, other[i]);
    nStored = other.size();

    return *this;
  }
  vector &operator=(vector &&other) {
    if (this == &other)
      return *this;

    if (alloc == other.alloc) {
      pstd::swap(ptr, other.ptr);
      pstd::swap(nAlloc, other.nAlloc);
      pstd::swap(nStored, other.nStored);
    } else {
      clear();
      reserve(other.size());
      for (size_t i = 0; i < other.size(); ++i)
        alloc.template construct<T>(ptr + i, std::move(other[i]));
      nStored = other.size();
    }

    return *this;
  }
  vector &operator=(std::initializer_list<T> &init) {
    reserve(init.size());
    clear();
    iterator iter = begin();
    for (const auto &value : init) {
      *iter = value;
      ++iter;
    }
    return *this;
  }

  void assign(size_type count, const T &value) {
    clear();
    reserve(count);
    for (size_t i = 0; i < count; ++i)
      push_back(value);
  }
  template <class InputIt>
  void assign(InputIt first, InputIt last) {
    LOG_FATAL("TODO");
    // TODO
  }
  void assign(std::initializer_list<T> &init) { assign(init.begin(), init.end()); }

  ~vector() {
    clear();
    alloc.deallocate_object(ptr, nAlloc);
  }

  MSK_CPU_GPU
  iterator begin() { return ptr; }
  MSK_CPU_GPU
  iterator end() { return ptr + nStored; }
  MSK_CPU_GPU
  const_iterator begin() const { return ptr; }
  MSK_CPU_GPU
  const_iterator end() const { return ptr + nStored; }
  MSK_CPU_GPU
  const_iterator cbegin() const { return ptr; }
  MSK_CPU_GPU
  const_iterator cend() const { return ptr + nStored; }

  MSK_CPU_GPU
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  MSK_CPU_GPU
  reverse_iterator rend() { return reverse_iterator(begin()); }
  MSK_CPU_GPU
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  MSK_CPU_GPU
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  allocator_type get_allocator() const { return alloc; }
  MSK_CPU_GPU
  size_t size() const { return nStored; }
  MSK_CPU_GPU
  bool empty() const { return size() == 0; }
  MSK_CPU_GPU
  size_t max_size() const { return (size_t)-1; }
  MSK_CPU_GPU
  size_t capacity() const { return nAlloc; }
  void reserve(size_t n) {
    if (nAlloc >= n)
      return;

    T *ra = alloc.template allocate_object<T>(n);
    for (int i = 0; i < nStored; ++i) {
      alloc.template construct<T>(ra + i, std::move(begin()[i]));
      alloc.destroy(begin() + i);
    }

    alloc.deallocate_object(ptr, nAlloc);
    nAlloc = n;
    ptr = ra;
  }
  // TODO: shrink_to_fit

  MSK_CPU_GPU
  reference operator[](size_type index) {
    DCHECK_LT(index, size());
    return ptr[index];
  }
  MSK_CPU_GPU
  const_reference operator[](size_type index) const {
    DCHECK_LT(index, size());
    return ptr[index];
  }
  MSK_CPU_GPU
  reference front() { return ptr[0]; }
  MSK_CPU_GPU
  const_reference front() const { return ptr[0]; }
  MSK_CPU_GPU
  reference back() { return ptr[nStored - 1]; }
  MSK_CPU_GPU
  const_reference back() const { return ptr[nStored - 1]; }
  MSK_CPU_GPU
  pointer data() { return ptr; }
  MSK_CPU_GPU
  const_pointer data() const { return ptr; }

  void clear() {
    for (int i = 0; i < nStored; ++i)
      alloc.destroy(&ptr[i]);
    nStored = 0;
  }

  iterator insert(const_iterator, const T &value) {
    // TODO
    LOG_FATAL("TODO");
  }
  iterator insert(const_iterator, T &&value) {
    // TODO
    LOG_FATAL("TODO");
  }
  iterator insert(const_iterator pos, size_type count, const T &value) {
    // TODO
    LOG_FATAL("TODO");
  }
  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    if (pos == end()) {
      size_t firstOffset = size();
      for (auto iter = first; iter != last; ++iter)
        push_back(*iter);
      return begin() + firstOffset;
    } else
      LOG_FATAL("TODO");
  }
  iterator insert(const_iterator pos, std::initializer_list<T> init) {
    // TODO
    LOG_FATAL("TODO");
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args) {
    // TODO
    LOG_FATAL("TODO");
  }
  template <class... Args>
  void emplace_back(Args &&... args) {
    if (nAlloc == nStored)
      reserve(nAlloc == 0 ? 4 : 2 * nAlloc);

    alloc.construct(ptr + nStored, std::forward<Args>(args)...);
    ++nStored;
  }

  iterator erase(const_iterator pos) {
    // TODO
    LOG_FATAL("TODO");
  }
  iterator erase(const_iterator first, const_iterator last) {
    // TODO
    LOG_FATAL("TODO");
  }

  void push_back(const T &value) {
    if (nAlloc == nStored)
      reserve(nAlloc == 0 ? 4 : 2 * nAlloc);

    alloc.construct(ptr + nStored, value);
    ++nStored;
  }
  void push_back(T &&value) {
    if (nAlloc == nStored)
      reserve(nAlloc == 0 ? 4 : 2 * nAlloc);

    alloc.construct(ptr + nStored, std::move(value));
    ++nStored;
  }
  void pop_back() {
    DCHECK(!empty());
    alloc.destroy(ptr + nStored - 1);
    --nStored;
  }

  void resize(size_type n) {
    if (n < size()) {
      for (size_t i = n; i < size(); ++i)
        alloc.destroy(ptr + i);
      if (n == 0) {
        alloc.deallocate_object(ptr, nAlloc);
        ptr = nullptr;
        nAlloc = 0;
      }
    } else if (n > size()) {
      reserve(n);
      for (size_t i = nStored; i < n; ++i)
        alloc.construct(ptr + i);
    }
    nStored = n;
  }
  void resize(size_type count, const value_type &value) {
    // TODO
    Fatal("TODO");
  }

  void swap(vector &other) {
    // TODO
    Fatal("TODO");
  }

 private:
  Allocator alloc;
  T *ptr = nullptr;
  size_t nAlloc = 0, nStored = 0;
};

}