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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/kosinski_moduled_compress.h"

#include "load_file_to_buffer.h"

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		fputs(
			"This tool compresses a supplied file in the Moduled Kosinski format. It tries to produce files accurate to Sega's original compressor.\n"
			"\n"
			"www.github.com/Clownacy/accurate-kosinski\n"
			"\n"
			"Usage: kosinskim_compress [in_file] [out_file](optional)\n"
			, stdout
		);
	}
	else
	{

		unsigned char *in_buffer;
		size_t in_size;

		if (LoadFileToBuffer(argv[1], &in_buffer, &in_size))
		{
		#ifdef DEBUG
			fprintf(stderr, "File '%s' with size %zX loaded\n", argv[1], in_size);
		#endif

			unsigned char *out_buffer;
			size_t out_size = KosinskiCompressModuled(in_buffer, in_size, &out_buffer);

			const char *out_filename = (argc > 2) ? argv[2] : "out.kosm";

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
			printf("Could not open '%s'\n", argv[1]);
		}
	}

	return exit_code;
}
