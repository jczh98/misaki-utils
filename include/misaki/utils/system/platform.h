#pragma once

namespace misaki::system {

// Platform flag
#ifdef _WIN32
#define MSK_PLATFORM_WINDOWS 1
#else
#define MSK_PLATFORM_WINDOWS 0
#endif
#ifdef __linux
#define MSK_PLATFORM_LINUX 1
#else
#define MSK_PLATFORM_LINUX 0
#endif
#ifdef __APPLE__
#define MSK_PLATFORM_APPLE 1
#else
#define MSK_PLATFORM_APPLE 0
#endif

#if MSK_PLATFORM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#if defined(_MSC_VER)
#define MSK_EXPORT __declspec(dllexport)
#define MSK_IMPORT __declspec(dllimport)
#define MSK_NOINLINE __declspec(noinline)
#define MSK_INLINE __forceinline
#define MSK_ALIGN(N) __declspec(align(N))
#define MSK_VECTORCALL __vectorcall
#else
#define MSK_EXPORT __attribute__((visibility("default")))
#define MSK_IMPORT
#define MSK_NOINLINE __attribute__((noinline))
#define MSK_INLINE __attribute__((always_inline)) inline
#define MSK_ALIGN(N) __attribute__((aligned(amt)))
#define MSK_VECTORCALL
#endif

/* Likely/unlikely macros (only on GCC/Clang) */
#if defined(__GNUG__) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

// For GPU
#if defined(__CUDA_ARCH__)
#define MSK_IS_GPU_CODE
#endif

#if defined(MSK_BUILD_ON_GPU) && defined(__CUDACC__)
#ifndef MSK_NOINLINE
#define MSK_NOINLINE __attribute__((noinline))
#endif
#define MSK_XPU __host__ __device__
#define MSK_GPU __device__
#if defined(MSK_IS_GPU_CODE)
#define MSK_CONST __device__ const
#else
#define MSK_CONST const
#endif
#else
#define MSK_CONST const
#define MSK_XPU
#define MSK_GPU
#endif

}  // namespace misaki::system