#ifndef OUTPUT_H
#define OUTPUT_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool writebytes(unsigned long long x, int nbytes);
bool chunked_write(unsigned long long (*rand64)(void), int nbytes,
                   int chunk_size);

#endif
