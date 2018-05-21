#include "kosinski_moduled.h"

#include <stdio.h>
#include <stdlib.h>

#include "kosinski_compress.h"
#include "kosinski_decompress.h"
#include "load_file_to_buffer.h"
#include "memory_stream.h"

size_t KosinskiCompressModuled(unsigned char *file_buffer, size_t file_size, unsigned char **p_output_buffer)
{
	MemoryStream *output_stream = MemoryStream_Init(0x100);

	MemoryStream_WriteByte(output_stream, file_size >> 8);
	MemoryStream_WriteByte(output_stream, file_size & 0xFF);

	const unsigned int extra_module_count = (file_size - 1) >> 12;

	for (unsigned int i = 0; i < extra_module_count; ++i)
	{
		unsigned char *compressed_buffer;
		size_t compressed_size = KosinskiCompress(file_buffer, 0x1000, &compressed_buffer);
		MemoryStream_WriteBytes(output_stream, compressed_buffer, compressed_size);
		file_buffer += 0x1000;
	}

	unsigned char *compressed_buffer;
	size_t compressed_size = KosinskiCompress(file_buffer, ((file_size - 1) & 0xFFF) + 1, &compressed_buffer);
	MemoryStream_WriteBytes(output_stream, compressed_buffer, compressed_size);

	size_t output_buffer_size = MemoryStream_GetIndex(output_stream);
	unsigned char *output_buffer = MemoryStream_GetBuffer(output_stream);

	free(output_stream);

	*p_output_buffer = output_buffer;
	return output_buffer_size;
}

void KosinskiDecompressModuled(unsigned char *in_file_buffer, FILE *out_file)
{
	const unsigned char byte1 = *in_file_buffer++;
	const unsigned short size = *in_file_buffer++ | (byte1 << 8);

	unsigned char *in_file_base = in_file_buffer;

	unsigned int extra_module_count = ((size - 1) >> 12);
	if (extra_module_count == 0xA)
		extra_module_count = 8;

	for (unsigned int i = 0; i < extra_module_count; ++i)
	{
		KosinskiDecompress(in_file_buffer, out_file);

		in_file_buffer += -(in_file_buffer - in_file_base) & 0xF;
	}

	KosinskiDecompress(in_file_buffer, out_file);
}
