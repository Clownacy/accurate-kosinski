#pragma once

#include <stddef.h>

size_t KosinskiCompress(unsigned char *file_buffer, size_t file_size, unsigned char **p_output_buffer);
