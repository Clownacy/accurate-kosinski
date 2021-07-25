// Copyright (c) 2018-2021 Clownacy

#include "load_file_to_buffer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

bool LoadFileToBuffer(const char *file_name, unsigned char **p_file_buffer, size_t *p_file_size)
{
	bool success = false;

	FILE *file = fopen(file_name, "rb");

	if (file != NULL)
	{
		fseek(file, 0, SEEK_END);
		const size_t file_size = ftell(file);
		rewind(file);

		unsigned char *file_buffer = malloc(file_size);

		if (file_buffer != NULL)
		{
			fread(file_buffer, file_size, 1, file);

			if (p_file_buffer != NULL)
				*p_file_buffer = file_buffer;

			if (p_file_size != NULL)
				*p_file_size = file_size;

			success = true;
		}

		fclose(file);
	}

	return success;
}
