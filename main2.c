#include <stdio.h>

#include "kosinski_decompress.h"

int main(int argc, char *argv[])
{
	FILE *in_file = fopen(argv[1], "rb");
	FILE *out_file = fopen("out.unc", "wb");

	KosinskiDecompress(in_file, out_file);

	fclose(in_file);
	fclose(out_file);
}