#pragma once

#include <stddef.h>

size_t KosinskiDecompress(unsigned char *in_file_buffer, unsigned char **out_file_buffer, size_t *out_file_size);
