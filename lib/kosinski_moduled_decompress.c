#include "kosinski_moduled_decompress.h"

#include <stddef.h>
#include <stdlib.h>

#include "kosinski_decompress.h"
#include "memory_stream.h"

size_t KosinskiDecompressModuled(unsigned char *in_file_buffer, unsigned char **out_file_buffer)
{
	MemoryStream *output_stream = MemoryStream_Init(0x100);

	const unsigned char byte1 = *in_file_buffer++;
	const unsigned short size = *in_file_buffer++ | (byte1 << 8);

	unsigned char *in_file_base = in_file_buffer;

	unsigned int extra_module_count = ((size - 1) >> 12);
	if (extra_module_count == 0xA)
		extra_module_count = 8;

	for (unsigned int i = 0; i < extra_module_count; ++i)
	{
		unsigned char *out_buffer;
		size_t out_size;
		in_file_buffer += KosinskiDecompress(in_file_buffer, &out_buffer, &out_size);
		in_file_buffer += -(in_file_buffer - in_file_base) & 0xF;

		MemoryStream_WriteBytes(output_stream, out_buffer, out_size);
	}

	unsigned char *out_buffer;
	size_t out_size;
	KosinskiDecompress(in_file_buffer, &out_buffer, &out_size);
	MemoryStream_WriteBytes(output_stream, out_buffer, out_size);

	size_t output_buffer_size = MemoryStream_GetIndex(output_stream);
	unsigned char *output_buffer = MemoryStream_GetBuffer(output_stream);

	free(output_stream);

	*out_file_buffer = output_buffer;
	return output_buffer_size;
}
