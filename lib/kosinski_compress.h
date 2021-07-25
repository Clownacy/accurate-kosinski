// Copyright (c) 2018-2021 Clownacy

#pragma once

#include <stddef.h>

size_t KosinskiCompress(const unsigned char *file_buffer, size_t file_size, unsigned char **output_buffer_pointer);
