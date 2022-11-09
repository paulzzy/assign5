#ifndef OPTIONS_H
#define OPTIONS_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct options {
  char *input;
  char *output;
  bool valid;
};

struct options parse_options(int argc, char *const *argv);

#endif
