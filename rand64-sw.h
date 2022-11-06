#ifndef RAND_64_SW_H
#define RAND_64_SW_H

#include <stdlib.h>
#include <stdio.h>

void software_rand64_init(void);
unsigned long long software_rand64(void);
void software_rand64_fini(void);

#endif
