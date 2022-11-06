#include "mrand48_r_wrapper.h"

static struct drand48_data buffer;

void mrand48_r_wrapper_init(void) { srand48_r((long int)time(NULL), &buffer); }

unsigned long long mrand48_r_wrapper(void) {
  long rand32_upper = 0;
  long rand32_lower = 0;

  mrand48_r(&buffer, &rand32_upper);
  mrand48_r(&buffer, &rand32_lower);

  unsigned long long rand64 = (long long)((unsigned long)rand32_upper << 32 |
                                          (unsigned long)rand32_lower);

  return rand64;
}

void mrand48_r_wrapper_fini(void) {}
