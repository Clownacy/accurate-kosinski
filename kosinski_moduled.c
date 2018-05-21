#include "kosinski_moduled.h"

#include <stdio.h>

#include "kosinski_compress.h"
#include "kosinski_decompress.h"
#include "load_file_to_memory.h"

void KosinskiCompressModuled(unsigned char *file_buffer, size_t file_size, FILE *output_file)
{
	fputc(file_size >> 8, output_file);
	fputc(file_size & 0xFF, output_file);

	const unsigned int extra_module_count = (file_size - 1) >> 12;

	for (unsigned int i = 0; i < extra_module_count; ++i)
	{
		unsigned char *compressed_buffer;
		size_t compressed_size = KosinskiCompress(file_buffer, 0x1000, &compressed_buffer);
		fwrite(compressed_buffer, compressed_size, 1, output_file);
		file_buffer += 0x1000;
	}

	unsigned char *compressed_buffer;
	size_t compressed_size = KosinskiCompress(file_buffer, ((file_size - 1) & 0xFFF) + 1, &compressed_buffer);
	fwrite(compressed_buffer, compressed_size, 1, output_file);
}

void KosinskiDecompressModuled(FILE *in_file, FILE *out_file)
{
	if (LoadFileToBuffer(argv[1], &file_buffer, NULL))

	const unsigned char byte1 = fgetc(in_file);
	const unsigned short size = fgetc(in_file) | (byte1 << 8);

	unsigned int extra_module_count = ((size - 1) >> 12);
	if (extra_module_count == 0xA)
		extra_module_count = 8;

	for (unsigned int i = 0; i < extra_module_count; ++i)
	{
		KosinskiDecompress(in_file, out_file);

		fseek(in_file, (-(ftell(in_file) - 2)) & 0xF, SEEK_CUR);
	}

	KosinskiDecompress(in_file, out_file);
}
