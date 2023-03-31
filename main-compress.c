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

static unsigned int ReadByte(void* const user_data)
{
	const int byte = fgetc((FILE*)user_data);

	return byte == EOF ? -1 : byte;
}

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
			"Usage: kosinski-compress [in_file] [out_file](optional)\n"
			, stdout
		);
	}
	else
	{
		FILE *in_file = fopen(argv[1], "rb");

		if (in_file == NULL)
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
		else
		{
			const char *out_filename;
			FILE *out_file;

		#ifdef DEBUG
			unsigned long file_size;
			fseek(in_file, 0, SEEK_END);
			file_size = ftell(in_file);
			rewind(in_file);

			fprintf(stderr, "File '%s' with size %lX loaded\n", argv[1], file_size);
		#endif

			out_filename = (argc > 2) ? argv[2] : "out.kos";
			out_file = fopen(out_filename, "wb");

			if (out_file == NULL)
			{
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "Could not open '%s'\n", out_filename);
			}
			else
			{
				KosinskiCompressCallbacks callbacks;
				callbacks.read_byte_user_data = in_file;
				callbacks.read_byte = ReadByte;
				callbacks.write_byte_user_data = out_file;
				callbacks.write_byte = WriteByte;

				KosinskiCompress(&callbacks,
				#ifdef DEBUG
					cc_true
				#else
					cc_false
				#endif
				);

				fclose(out_file);
			}

			fclose(in_file);
		}
	}

	return exit_code;
}
