#include <stdio.h>
#include <stdlib.h>

#include "kosinski_moduled.h"
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
		unsigned char *file_buffer;

		if (LoadFileToBuffer(argv[1], &file_buffer, NULL))
		{
			char *out_filename = (argc > 2) ? argv[2] : "out.unc";

			FILE *out_file = fopen(out_filename, "wb");

			if (out_file)
			{
				KosinskiDecompressModuled(file_buffer, out_file);

				fclose(out_file);

				success = EXIT_SUCCESS;
			}
			else
			{
				printf("Could not open '%s'\n", out_filename);
			}

			free(file_buffer);
		}
		else
		{
			printf("Could not open '%s'\n", argv[1]);
		}
	}

	return success;
}
