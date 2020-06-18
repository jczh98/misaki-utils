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

}  // namespace misaki::system