// Copyright (c) 2018 Clownacy

#include "load_file_to_buffer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool LoadFileToBuffer(char *file_name, unsigned char **p_file_buffer, long int *p_file_size)
{
	bool success = false;

	unsigned char *file_buffer;
	long int file_size;

	FILE *file = fopen(file_name, "rb");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		file_size = ftell(file);
		rewind(file);

		file_buffer = malloc(file_size);

		if (file_buffer)
		{
			fread(file_buffer, file_size, 1, file);

			success = true;
		}

		fclose(file);

		if (p_file_buffer)
			*p_file_buffer = file_buffer;

		if (p_file_size)
			*p_file_size = file_size;
	}

	return success;
}
