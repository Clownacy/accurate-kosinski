#pragma once

#include <stdio.h>

void KosinskiCompressModuled(unsigned char *file_buffer, size_t file_size, FILE *output_file);
void KosinskiDecompressModuled(FILE *in_file, FILE *out_file);
