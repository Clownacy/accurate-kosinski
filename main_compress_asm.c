// Copyright (c) 2018-2021 Clownacy

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/kosinski_compress.h"

#include "load_file_to_buffer.h"

#ifdef __MINGW32__
#define FPRINTF __mingw_fprintf
#else
#define FPRINTF fprintf
#endif

int main(int argc, char **argv)
{
	int success = EXIT_FAILURE;

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
			printf("File '%s' with size %lX loaded\n", argv[1], file_size);
		#endif

			unsigned char *out_buffer;
			size_t out_size = KosinskiCompress(file_buffer, file_size, &out_buffer);

			char *out_filename = (argc > 2) ? argv[2] : "out.asm";

			FILE *out_file = fopen(out_filename, "w");

			if (out_file != NULL)
			{
				size_t claimed_out_size = (out_size + 0x100) & ~0xFF;
				// Shift-JIS: Supposedly translates to 'Before compression', 'After compression', 'Compression ratio', and 'Number of cells'
				FPRINTF(out_file, "; à≥èkëO $%lx  à≥èkå„ $%zx  à≥èkó¶ %.1f%%  ÉZÉãêî %ld", file_size, claimed_out_size, ((float)claimed_out_size / file_size) * 100, file_size / 32);

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

				success = EXIT_SUCCESS;
			}
			else
			{
				printf("Could not open '%s'\n", out_filename);
			}

			free(file_buffer);
		}
		else
		{
			printf("Could not open '%s'\n", argv[1]);
		}
	}

	return success;
}
