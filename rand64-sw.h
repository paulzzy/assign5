#ifndef RAND_64_SW_H
#define RAND_64_SW_H

#include <stdio.h>
#include <stdlib.h>

void software_rand64_set_file(char* file);
void software_rand64_init(void);
unsigned long long software_rand64(void);
void software_rand64_fini(void);

#endif
