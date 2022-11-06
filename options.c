#include "options.h"

struct options parse_options(int argc, char *const *argv) {
  struct options options = {.input = NULL, .output = NULL, .valid = true};
  char option;

  while ((option = getopt(argc, argv, "i:o:")) != -1) {
    switch (option) {
      case 'i':
        options.input = optarg;
        break;
      case 'o':
        options.output = optarg;
        break;
      default:
        options.valid = false;
    }
  }

  return options;
}
