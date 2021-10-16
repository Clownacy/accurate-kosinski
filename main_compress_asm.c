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

#include "lib/kosinski_compress.h"

#include "load_file_to_buffer.h"

int main(int argc, char **argv)
{
	int exit_code = EXIT_FAILURE;

	if (argc < 2)
	{
		printf(
		"This tool compresses a supplied file in the Kosinski format. It tries to produce files accurate to Sega's original compressor.\n"
		"Made by Clownacy.\n"
		"\n"
		"Usage: kosinski_compress.exe [in_file] [out_file](optional)\n"
		);
	}
	else
	{
		unsigned char *file_buffer;
		size_t file_size;

		if (LoadFileToBuffer(argv[1], &file_buffer, &file_size))
		{
		#ifdef DEBUG
			fprintf(stderr, "File '%s' with size %lX loaded\n", argv[1], file_size);
		#endif

			unsigned char *out_buffer;
			size_t out_size = KosinskiCompress(file_buffer, file_size, &out_buffer);

			char *out_filename = (argc > 2) ? argv[2] : "out.asm";

			FILE *out_file = fopen(out_filename, "w");

			if (out_file != NULL)
			{
				size_t claimed_out_size = (out_size + 0x100) & ~0xFF;
				// Shift-JIS: Supposedly translates to 'Before compression', 'After compression', 'Compression ratio', and 'Number of cells'
				fprintf(out_file, "; \x88\xB3\x8F\x6B\x91\x4F $%zx  \x88\xB3\x8F\x6B\x8C\xE3 $%zx  \x88\xB3\x8F\x6B\x97\xA6 %.1f%%  \x83\x5A\x83\x8B\x90\x94 %zd", file_size, claimed_out_size, ((float)claimed_out_size / file_size) * 100, file_size / 32);

				unsigned int index = 0;
				for (size_t bytes_remaining = out_size; bytes_remaining != 0; bytes_remaining -= 0x10)
				{
					fprintf(out_file, "\n	dc.b	");

					fprintf(out_file, "$%.2x", out_buffer[index++]);

					for (unsigned int i = 1; i < 0x10; ++i)
					{
						fprintf(out_file, ",$%.2x", out_buffer[index++]);
					}
				}

				free(out_buffer);
				fclose(out_file);
			}
			else
			{
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "Could not open '%s'\n", out_filename);
			}

			free(file_buffer);
		}
		else
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
	}

	return exit_code;
}
