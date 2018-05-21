#pragma once

#include <stddef.h>

size_t KosinskiCompressModuled(unsigned char *file_buffer, size_t file_size, unsigned char **p_output_buffer);
size_t KosinskiDecompressModuled(unsigned char *in_file_buffer, unsigned char **out_file_buffer);
