// Copyright (c) 2018-2021 Clownacy

#include "kosinski_moduled_compress.h"

#include <stddef.h>
#include <stdlib.h>

#include "kosinski_compress.h"
#include "memory_stream.h"

size_t KosinskiCompressModuled(const unsigned char *file_buffer, size_t file_size, unsigned char **p_output_buffer)
{
	if (file_size > 0xFFFF)
	{
		return 0;	// Cannot fit size of file in header - give up
	}
	else
	{
		MemoryStream *output_stream = MemoryStream_Create(0x100, false);

		MemoryStream_WriteByte(output_stream, file_size >> 8);
		MemoryStream_WriteByte(output_stream, file_size & 0xFF);

		const size_t extra_module_count = (file_size - 1) >> 12;

		for (size_t i = 0; i < extra_module_count; ++i)
		{
			unsigned char *compressed_buffer;
			const size_t compressed_size = KosinskiCompress(file_buffer, 0x1000, &compressed_buffer);
			MemoryStream_WriteBytes(output_stream, compressed_buffer, compressed_size);
			file_buffer += 0x1000;
		}

		unsigned char *compressed_buffer;
		const size_t compressed_size = KosinskiCompress(file_buffer, ((file_size - 1) & 0xFFF) + 1, &compressed_buffer);
		MemoryStream_WriteBytes(output_stream, compressed_buffer, compressed_size);

		const size_t output_buffer_size = MemoryStream_GetPosition(output_stream);
		unsigned char *output_buffer = MemoryStream_GetBuffer(output_stream);

		MemoryStream_Destroy(output_stream);

		if (p_output_buffer != NULL)
			*p_output_buffer = output_buffer;

		return output_buffer_size;
	}
}
