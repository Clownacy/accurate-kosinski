// Copyright (c) 2018-2021 Clownacy

#pragma once

#include <stdbool.h>
#include <stddef.h>

bool LoadFileToBuffer(const char *file_name, unsigned char **file_buffer, size_t *file_size);
