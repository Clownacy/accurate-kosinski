/*
 * zlib License
 *
 * (C) 2018-2021 Clownacy
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

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
