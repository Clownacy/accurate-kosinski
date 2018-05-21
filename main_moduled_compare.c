#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_file_to_buffer.h"
#include "kosinski_moduled.h"

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		unsigned char *in_file_buffer;
		long int in_file_size;
		if (LoadFileToBuffer(argv[i], &in_file_buffer, &in_file_size))
		{
			unsigned char *uncompressed_buffer;
			size_t uncompressed_size = KosinskiDecompressModuled(in_file_buffer, &uncompressed_buffer);

			printf("File '%s' with size %X loaded\n", argv[i], uncompressed_size);

			unsigned char *compressed_buffer;
			long int compressed_size = KosinskiCompressModuled(uncompressed_buffer, uncompressed_size, &compressed_buffer);

			free(uncompressed_buffer);

			if (in_file_size != compressed_size)
				printf("File sizes don't match!\n");

			if (memcmp(in_file_buffer, compressed_buffer, (in_file_size > compressed_size) ? compressed_size : in_file_size))
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

	getchar();
}
