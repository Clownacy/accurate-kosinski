#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "kosinski_compress.h"

bool LoadFileToBuffer(char *file_name, unsigned char **file_buffer, size_t *file_size)
{
	bool success = false;

	FILE *file = fopen(file_name, "rb");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		*file_size = ftell(file);
		rewind(file);

		*file_buffer = malloc(*file_size);

		if (*file_buffer)
		{
			fread(*file_buffer, *file_size, 1, file);

			success = true;
		}

		fclose(file);
	}

	return success;
}

int main(int argc, char *argv[])
{
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
		int success = EXIT_FAILURE;

		unsigned char *file_buffer;
		size_t file_size;

		if (LoadFileToBuffer(argv[1], &file_buffer, &file_size))
		{
			#ifndef SHUTUP
			printf("File '%s' with size %X loaded\n", argv[1], file_size);
			#endif
			FILE *dst_file = fopen((argc > 2) ? argv[2] : "out.kos", "wb");
			KosinskiCompress(file_buffer, file_size, dst_file);
			fclose(dst_file);

			success = EXIT_SUCCESS;
		}

		return success;
	}
}
