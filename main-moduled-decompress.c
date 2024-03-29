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

#include "lib/kosinski-moduled-decompress.h"

static unsigned int ReadByte(void* const user_data)
{
	return fgetc((FILE*)user_data);
}

static void WriteByte(void* const user_data, const unsigned int byte)
{
	fputc(byte, (FILE*)user_data);
}

int main(const int argc, char** const argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		fputs(
			"This tool decompresses a supplied file that's in the Moduled Kosinski format.\n"
			"\n"
			"www.github.com/Clownacy/accurate-kosinski\n"
			"\n"
			"Usage: kosinskim-decompress [in_file] [out_file](optional)\n"
			, stdout
		);
	}
	else
	{
		FILE* const in_file = fopen(argv[1], "rb");

		if (in_file == NULL)
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
		else
		{
			const char* const out_filename = (argc > 2) ? argv[2] : "out.unc";
			FILE* const out_file = fopen(out_filename, "wb");

			if (out_file == NULL)
			{
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "Could not open '%s'\n", out_filename);
			}
			else
			{
				KosinskiDecompressCallbacks callbacks;
				callbacks.read_byte_user_data = in_file;
				callbacks.read_byte = ReadByte;
				callbacks.write_byte_user_data = out_file;
				callbacks.write_byte = WriteByte;

				KosinskiDecompressModuled(&callbacks,
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
