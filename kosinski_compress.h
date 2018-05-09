#pragma once

#include <stddef.h>
#include <stdio.h>

void KosinskiCompress(unsigned char *file_buffer, size_t file_size, FILE *output_file);
