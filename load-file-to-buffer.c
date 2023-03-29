/*
Copyright (c) 2018-2021 Clownacy

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/

#include "load-file-to-buffer.h"

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

		unsigned char *file_buffer = (unsigned char*)malloc(file_size);

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
