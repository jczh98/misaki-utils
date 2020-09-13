#pragma once

#include <stdint.h>

#include "common.hpp"

namespace misaki::math {

#define PCG32_DEFAULT_STATE 0x853c49e6748fea9bULL
#define PCG32_DEFAULT_STREAM 0xda3e39cb94b95bdbULL
#define PCG32_MULT 0x5851f42d4c957f2dULL

struct PCG32 {
  MSK_XPU PCG32() : state(PCG32_DEFAULT_STATE), inc(PCG32_DEFAULT_STREAM) {}
  MSK_XPU PCG32(uint64_t initstate, uint64_t initseq = 1u) { seed(initstate, initseq); }

  MSK_XPU void seed(uint64_t initstate, uint64_t initseq = 1) {
    state = 0U;
    inc = (initseq << 1u) | 1u;
    next_uint32();
    state += initstate;
    next_uint32();
  }
  MSK_XPU uint32_t next_uint32() {
    uint64_t oldstate = state;
    state = oldstate * PCG32_MULT + inc;
    uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
    uint32_t rot = (uint32_t)(oldstate >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
  }

  MSK_XPU float next_float32() {
    /* Trick from MTGP: generate an uniformly distributed
           single precision number in [1,2) and subtract 1. */
    union {
      uint32_t u;
      float f;
    } x;
    x.u = (next_uint32() >> 9) | 0x3f800000u;
    return x.f - 1.0f;
  }

  MSK_XPU double next_float64() {
    /* Trick from MTGP: generate an uniformly distributed
           double precision number in [1,2) and subtract 1. */
    union {
      uint64_t u;
      double d;
    } x;
    x.u = ((uint64_t)next_uint32() << 20) | 0x3ff0000000000000ULL;
    return x.d - 1.0;
  }

  bool operator==(const PCG32 &other) const { return state == other.state && inc == other.inc; }
  bool operator!=(const PCG32 &other) const { return state != other.state || inc != other.inc; }

  uint64_t state;  // RNG state.  All values are possible.
  uint64_t inc;    // Controls which RNG sequence (stream) is selected. Must *always* be odd.
};

}  // namespace misaki::math