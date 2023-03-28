/*
Copyright (c) 2018-2021 Clownacy

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/

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
		MemoryStream output_stream;
		MemoryStream_Create(&output_stream, CC_FALSE);

		MemoryStream_WriteByte(&output_stream, (file_size >> 8) & 0xFF);
		MemoryStream_WriteByte(&output_stream, (file_size >> 0) & 0xFF);

		const size_t extra_module_count = (file_size - 1) >> 12;

		for (size_t i = 0; i < extra_module_count; ++i)
		{
			unsigned char *compressed_buffer;
			const size_t compressed_size = KosinskiCompress(file_buffer, 0x1000, &compressed_buffer);
			MemoryStream_Write(&output_stream, compressed_buffer, 1, compressed_size);
			free(compressed_buffer);
			file_buffer += 0x1000;
		}

		unsigned char *compressed_buffer;
		const size_t compressed_size = KosinskiCompress(file_buffer, ((file_size - 1) & 0xFFF) + 1, &compressed_buffer);
		MemoryStream_Write(&output_stream, compressed_buffer, 1, compressed_size);
		free(compressed_buffer);

		const size_t output_buffer_size = MemoryStream_GetPosition(&output_stream);
		unsigned char *output_buffer = MemoryStream_GetBuffer(&output_stream);

		MemoryStream_Destroy(&output_stream);

		if (p_output_buffer != NULL)
			*p_output_buffer = output_buffer;

		return output_buffer_size;
	}
}
