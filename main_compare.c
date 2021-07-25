// Copyright (c) 2018-2021 Clownacy

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

#ifdef __MINGW32__
#define PRINTF __mingw_printf
#else
#define PRINTF printf
#endif

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		unsigned char *in_file_buffer;
		size_t in_file_size;
		if (LoadFileToBuffer(argv[i], &in_file_buffer, &in_file_size))
		{
			unsigned char *uncompressed_buffer;
			size_t uncompressed_size;
			KosinskiDecompress(in_file_buffer, &uncompressed_buffer, &uncompressed_size);

			PRINTF("File '%s' with size %zX loaded\n", argv[i], uncompressed_size);

			unsigned char *compressed_buffer;
			const size_t compressed_size = KosinskiCompress(uncompressed_buffer, uncompressed_size, &compressed_buffer);

			free(uncompressed_buffer);

			if (in_file_size != compressed_size)
				printf("File sizes don't match!\n");

			if (memcmp(in_file_buffer, compressed_buffer, MIN(in_file_size, compressed_size)))
				printf("The files don't match!\n\n");
			else
				printf("Yay the files match.\n\n");

			free(compressed_buffer);
			free(in_file_buffer);
		}
		else
		{
			printf("Could not open '%s'\n", argv[i]);
		}
	}

#ifdef _WIN32
	getchar();
#endif
}
