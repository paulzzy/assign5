#include "rand64-sw.h"

// Filename of input stream
static char* stream_source;

/* Input stream containing random bytes.  */
static FILE* urandstream;

void software_rand64_set_file(char* file) { stream_source = file; }

/* Initialize the software rand64 implementation.  */
void software_rand64_init(void) {
  urandstream = fopen(stream_source, "r");
  if (!urandstream) abort();
}

/* Return a random value, using software operations.  */
unsigned long long software_rand64(void) {
  unsigned long long int x;
  if (fread(&x, sizeof x, 1, urandstream) != 1) abort();
  return x;
}

/* Finalize the software rand64 implementation.  */
void software_rand64_fini(void) { fclose(urandstream); }
