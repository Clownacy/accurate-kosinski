#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/kosinski_decompress.h"

#include "load_file_to_buffer.h"

int main(int argc, char *argv[])
{
	int success = EXIT_FAILURE;

	if (argc < 2)
	{
		printf("Gimme a file, dumbass\n");
	}
	else
	{
		unsigned char *in_buffer;

		if (LoadFileToBuffer(argv[1], &in_buffer, NULL))
		{
			unsigned char *out_buffer;
			size_t out_size;
			KosinskiDecompress(in_buffer, &out_buffer, &out_size);

			char *out_filename = (argc > 2) ? argv[2] : "out.unc";

			FILE *out_file = fopen(out_filename, "wb");

			if (out_file)
			{
				fwrite(out_buffer, out_size, 1, out_file);
				free(out_buffer);
				fclose(out_file);

				success = EXIT_SUCCESS;
			}
			else
			{
				printf("Could not open '%s'\n", out_filename);
			}

			free(in_buffer);
		}
		else
		{
			printf("Could not open '%s'\n", argv[1]);
		}
	}

	return success;
}
