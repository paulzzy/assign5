/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "mrand48_r_wrapper.h"
#include "options.h"
#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"

/* Main program, which outputs N bytes of random data.  */
int main(int argc, char** argv) {
  /* Check arguments.  */
  struct options options = parse_options(argc, argv);

  if (!options.valid) {
    return 1;
  }

  bool valid = false;
  long long nbytes;

  if (argc >= 2) {
    char* endptr;
    errno = 0;
    nbytes = strtoll(argv[optind], &endptr, 10);
    if (errno) {
      perror(argv[1]);
    } else {
      valid = !*endptr && 0 <= nbytes;
    }
  }

  if (!valid) {
    fprintf(stderr, "%s: usage: %s [-i input] [-o output] NBYTES\n", argv[0],
            argv[0]);
    return 1;
  }

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize)(void);
  unsigned long long (*rand64)(void);
  void (*finalize)(void);

  // Implement -i option
  if (options.input == NULL || strcmp(options.input, "rdrand") == 0) {
    if (rdrand_supported()) {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    } else {
      fprintf(stderr,
              "%s: `rdrand` not supported. Please select another random source "
              "using `-i`.\n",
              argv[0]);
      return 1;
    }
  } else if (strcmp(options.input, "mrand48_r") == 0) {
    initialize = mrand48_r_wrapper_init;
    rand64 = mrand48_r_wrapper;
    finalize = mrand48_r_wrapper_fini;
  } else {
    if (options.input[0] == '/') {
      software_rand64_set_file(options.input);
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
    } else {
      fprintf(stderr, "%s: Unrecognized `-i` argument: %s\n", argv[0],
              options.input);
      return 1;
    }
  }

  // Implement -o option
  const int base = 10;
  const char string_terminator = '\0';

  int chunk_size = 0;
  char* end = NULL;

  if (options.output == NULL || strcmp(options.output, "stdio") == 0) {
    chunk_size = -1;
  } else {
    int parsed_num = strtoull(options.output, &end, base);

    if (parsed_num > 0 && *end == string_terminator) {
      chunk_size = parsed_num;
    } else {
      fprintf(stderr, "%s: Unrecognized `-o` argument: %s\n", argv[0],
              options.output);
      return 1;
    }
  }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0) return 0;

  initialize();
  int wordsize = sizeof rand64();
  int output_errno = 0;

  if (chunk_size == -1) {
    do {
      unsigned long long x = rand64();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes(x, outbytes)) {
        output_errno = errno;
        break;
      }
      nbytes -= outbytes;
    } while (0 < nbytes);
  } else {
    chunked_write(rand64, nbytes, chunk_size);
  }

  if (fclose(stdout) != 0) output_errno = errno;

  if (output_errno) {
    errno = output_errno;
    perror("output");
  }

  finalize();
  return !!output_errno;
}
