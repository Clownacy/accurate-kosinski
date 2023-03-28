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
#include <string.h>

#include "lib/kosinski_compress.h"
#include "lib/kosinski_decompress.h"

#include "load_file_to_buffer.h"

#undef MIN
#undef MAX
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	for (int i = 1; i < argc; ++i)
	{
		unsigned char *in_file_buffer;
		size_t in_file_size;
		if (LoadFileToBuffer(argv[i], &in_file_buffer, &in_file_size))
		{
			unsigned char *uncompressed_buffer;
			size_t uncompressed_size;
			KosinskiDecompress(in_file_buffer, &uncompressed_buffer, &uncompressed_size);

		#ifdef DEBUG
			fprintf(stderr, "File '%s' with size %zX loaded\n", argv[i], uncompressed_size);
		#endif

			unsigned char *compressed_buffer;
			const size_t compressed_size = KosinskiCompress(uncompressed_buffer, uncompressed_size, &compressed_buffer);

			free(uncompressed_buffer);

			if (in_file_size != compressed_size)
			{
				exit_code = EXIT_FAILURE;
				fputs("File sizes don't match!\n", stdout);
			}

			if (memcmp(in_file_buffer, compressed_buffer, MIN(in_file_size, compressed_size)))
			{
				exit_code = EXIT_FAILURE;
				fputs("The files don't match!\n\n", stdout);
			}
			else
			{
				fputs("Yay the files match.\n\n", stdout);
			}

			free(compressed_buffer);
			free(in_file_buffer);
		}
		else
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[i]);
		}
	}

	return exit_code;
}
