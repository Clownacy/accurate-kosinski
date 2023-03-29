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

static void WriteByte(void* const user_data, const unsigned int byte)
{
	fputc(byte, (FILE*)user_data);
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		fputs(
			"This tool compresses a supplied file in the Kosinski format. It tries to produce files accurate to Sega's original compressor.\n"
			"\n"
			"www.github.com/Clownacy/accurate-kosinski\n"
			"\n"
			"Usage: kosinski_compress [in_file] [out_file](optional)\n"
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
			fprintf(stderr, "File '%s' with size %zX loaded\n", argv[1], file_size);
		#endif

			const char *out_filename = (argc > 2) ? argv[2] : "out.kos";

			FILE *out_file = fopen(out_filename, "wb");

			if (out_file != NULL)
			{
				KosinskiCompress(file_buffer, file_size, WriteByte, out_file,
				#ifdef DEBUG
					true
				#else
					false
				#endif
				);

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
