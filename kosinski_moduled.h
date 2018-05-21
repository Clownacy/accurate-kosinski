#pragma once

#include <stdio.h>

size_t KosinskiCompressModuled(unsigned char *file_buffer, size_t file_size, unsigned char **p_output_buffer);
void KosinskiDecompressModuled(unsigned char *in_file_buffer, FILE *out_file);
