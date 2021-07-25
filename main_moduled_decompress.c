// Copyright (c) 2018-2021 Clownacy

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/kosinski_moduled_decompress.h"

#include "load_file_to_buffer.h"

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		fputs("Gimme a file, dumbass\n", stdout);
	}
	else
	{
		unsigned char *in_buffer;

		if (LoadFileToBuffer(argv[1], &in_buffer, NULL))
		{
			unsigned char *out_buffer;
			const size_t out_size = KosinskiDecompressModuled(in_buffer, &out_buffer);

			const char *out_filename = (argc > 2) ? argv[2] : "out.unc";

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
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
	}

	return exit_code;
}
