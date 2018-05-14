#include <stdio.h>
#include <stdlib.h>

#include "kosinski_moduled.h"

int main(int argc, char *argv[])
{
	int success = EXIT_FAILURE;

	if (argc < 2)
	{
		printf("Gimme a file, dumbass\n");
	}
	else
	{
		FILE *in_file = fopen(argv[1], "rb");

		if (in_file)
		{
			char *out_filename = (argc > 2) ? argv[2] : "out.unc";

			FILE *out_file = fopen(out_filename, "wb");

			if (out_file)
			{
				KosinskiDecompressModuled(in_file, out_file);

				fclose(out_file);

				success = EXIT_SUCCESS;
			}
			else
			{
				printf("Could not open '%s'\n", out_filename);
			}

			fclose(in_file);
		}
		else
		{
			printf("Could not open '%s'\n", argv[1]);
		}
	}

	return success;
}
