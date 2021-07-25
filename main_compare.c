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
				fputs("File sizes don't match!\n", stdout);

			if (memcmp(in_file_buffer, compressed_buffer, MIN(in_file_size, compressed_size)))
				fputs("The files don't match!\n\n", stdout);
			else
				fputs("Yay the files match.\n\n", stdout);

			free(compressed_buffer);
			free(in_file_buffer);
		}
		else
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[i]);
		}
	}

#ifdef _WIN32
	getchar();
#endif

	return exit_code;
}
