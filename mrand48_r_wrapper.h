#ifndef MRAND48_R_WRAPPER_H
#define MRAND48_R_WRAPPER_H

#include <stdlib.h>
#include <time.h>

void mrand48_r_wrapper_init(void);
unsigned long long mrand48_r_wrapper(void);
void mrand48_r_wrapper_fini(void);

#endif
