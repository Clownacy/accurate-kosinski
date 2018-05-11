#include <stdio.h>

#include "kosinski_decompress.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Gimme a file, dumbass\n");
	}
	else
	{
		FILE *in_file = fopen(argv[1], "rb");
		FILE *out_file = fopen("out.unc", "wb");

		KosinskiDecompress(in_file, out_file);

		fclose(in_file);
		fclose(out_file);
	}
}
