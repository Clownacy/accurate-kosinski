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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/kosinski-compress.h"

#include "load-file-to-buffer.h"
#include "memory-stream.h"

static MemoryStream memory_stream;

static void WriteByte(void* const user_data, const unsigned int byte)
{
	MemoryStream_WriteByte((MemoryStream*)user_data, byte);
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_FAILURE;

	if (argc < 2)
	{
		fputs(
			"This tool compresses a supplied file in the Kosinski format. It tries to produce files accurate to Sega's original compressor.\n"
			"This particular tool outputs an assembly file, matching the format of the one found in Sonic 2's 'Simon Wai' prototype.\n"
			"\n"
			"www.github.com/Clownacy/accurate-kosinski\n"
			"\n"
			"Usage: kosinski_compress_asm [in_file] [out_file](optional)\n"
			, stdout
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

			MemoryStream_Create(&memory_stream, cc_true);

			KosinskiCompress(file_buffer, file_size, WriteByte, &memory_stream,
			#ifdef DEBUG
				true
			#else
				false
			#endif
			);

			free(file_buffer);

			const char *out_filename = (argc > 2) ? argv[2] : "out.asm";

			FILE *out_file = fopen(out_filename, "w");

			if (out_file != NULL)
			{
				const unsigned char* const out_buffer = MemoryStream_GetBuffer(&memory_stream);
				const size_t out_size = MemoryStream_GetPosition(&memory_stream);

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

				fclose(out_file);
			}
			else
			{
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "Could not open '%s'\n", out_filename);
			}

			MemoryStream_Destroy(&memory_stream);
		}
		else
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
	}

	return exit_code;
}
