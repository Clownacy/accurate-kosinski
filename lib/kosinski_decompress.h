// Copyright (c) 2018-2021 Clownacy

#pragma once

#include <stddef.h>

size_t KosinskiDecompress(const unsigned char *in_file_buffer, unsigned char **out_file_buffer, size_t *out_file_size);
