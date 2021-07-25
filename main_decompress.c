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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/kosinski_decompress.h"

#include "load_file_to_buffer.h"

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		fputs("Gimme a file, dumbass\n", stdout);
	}
	else
	{
		unsigned char *in_buffer;

		if (LoadFileToBuffer(argv[1], &in_buffer, NULL))
		{
			unsigned char *out_buffer;
			size_t out_size;
			KosinskiDecompress(in_buffer, &out_buffer, &out_size);

			const char *out_filename = (argc > 2) ? argv[2] : "out.unc";

			FILE *out_file = fopen(out_filename, "wb");

			if (out_file != NULL)
			{
				fwrite(out_buffer, out_size, 1, out_file);
				free(out_buffer);
				fclose(out_file);
			}
			else
			{
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "Could not open '%s'\n", out_filename);
			}

			free(in_buffer);
		}
		else
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
	}

	return exit_code;
}
