/*
Copyright (c) 2018-2023 Clownacy

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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

cc_bool LoadFileToBuffer(const char* const file_name, unsigned char** const p_file_buffer, size_t* const p_file_size)
{
	cc_bool success = cc_false;

	FILE* const file = fopen(file_name, "rb");

	if (file != NULL)
	{
		size_t file_size;
		unsigned char *file_buffer;

		fseek(file, 0, SEEK_END);
		file_size = ftell(file);
		rewind(file);

		file_buffer = (unsigned char*)malloc(file_size);

		if (file_buffer != NULL)
		{
			fread(file_buffer, file_size, 1, file);

			if (p_file_buffer != NULL)
				*p_file_buffer = file_buffer;

			if (p_file_size != NULL)
				*p_file_size = file_size;

			success = cc_true;
		}

		fclose(file);
	}

	return success;
}
