#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "kosinski.h"

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
	int success = EXIT_FAILURE;

	unsigned char *file_buffer;
	size_t file_size;

	if (LoadFileToBuffer(argv[1], &file_buffer, &file_size))
	{
		printf("File '%s' with size %X loaded\n", argv[1], file_size);
		FILE *dst_file = fopen("out.kos", "wb");
		KosinskiCompress(file_buffer, file_size, dst_file);
		fclose(dst_file);

		success = EXIT_SUCCESS;
	}

	return success;
}
