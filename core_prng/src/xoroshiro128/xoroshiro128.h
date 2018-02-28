#include <stdint.h>

typedef struct s_xoroshiro128_state {
  uint64_t s[2];
  int has_uint32;
  uint32_t uinteger;
} xoroshiro128_state;

static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

static inline uint64_t xoroshiro128_next(uint64_t *s) {
  const uint64_t s0 = s[0];
  uint64_t s1 = s[1];
  const uint64_t result = s0 + s1;

  s1 ^= s0;
  s[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
  s[1] = rotl(s1, 36);                   // c

  return result;
}

static inline uint64_t xoroshiro128_next64(xoroshiro128_state *state) {
  return xoroshiro128_next(&state->s[0]);
}

static inline uint64_t xoroshiro128_next32(xoroshiro128_state *state) {
  if (state->has_uint32) {
    state->has_uint32 = 0;
    return state->uinteger;
  }
  uint64_t next = xoroshiro128_next(&state->s[0]);
  state->has_uint32 = 1;
  state->uinteger = (uint32_t)(next & 0xffffffff);
  return (uint32_t)(next >> 32);
}

void xoroshiro128_jump(xoroshiro128_state *state);