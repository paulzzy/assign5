#include "output.h"

static int write_random_chunk(unsigned long long (*rand64)(void),
                              int chunk_size) {
  unsigned char *output_buf = malloc(chunk_size);

  if (!output_buf) {
    return -1;
  }

  const int num_8_byte_chunks = chunk_size / 8;
  const int stdout_fd = 1;

  // Fill buffer up to a multiple of 8, since `rand64` returns 8 bytes rather
  // than 1 byte
  for (int i = 0; i < num_8_byte_chunks; i++) {
    unsigned long long rand_8_bytes = rand64();

    output_buf[i] = (unsigned char)(rand_8_bytes);
    output_buf[i + 1] = (unsigned char)(rand_8_bytes >> 8);
    output_buf[i + 2] = (unsigned char)(rand_8_bytes >> 16);
    output_buf[i + 3] = (unsigned char)(rand_8_bytes >> 24);
    output_buf[i + 4] = (unsigned char)(rand_8_bytes >> 32);
    output_buf[i + 5] = (unsigned char)(rand_8_bytes >> 40);
    output_buf[i + 6] = (unsigned char)(rand_8_bytes >> 48);
    output_buf[i + 7] = (unsigned char)(rand_8_bytes >> 56);
  }

  // Fill remainder of buffer
  if (chunk_size % 8 > 0) {
    unsigned long long rand_8_bytes = rand64();
    for (int i = num_8_byte_chunks * 8; i < chunk_size % 8; i++) {
      output_buf[i] = (unsigned char)(rand_8_bytes >> (i * 8));
    }
  }

  int bytes_written = write(stdout_fd, output_buf, chunk_size);
  free(output_buf);

  return bytes_written;
}

bool writebytes(unsigned long long x, int nbytes) {
  do {
    if (putchar(x) < 0) return false;
    x >>= CHAR_BIT;
    nbytes--;
  } while (0 < nbytes);

  return true;
}

bool chunked_write(unsigned long long (*rand64)(void), int nbytes,
                   int chunk_size) {
  while (nbytes > 0) {
    chunk_size = (nbytes < chunk_size) ? nbytes : chunk_size;

    int bytes_written = write_random_chunk(rand64, chunk_size);

    if (bytes_written == -1) {
      return false;
    }

    nbytes -= bytes_written;
  }

  return true;
}
