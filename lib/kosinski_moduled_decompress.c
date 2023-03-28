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

#include "kosinski_moduled_decompress.h"

#include <stddef.h>
#include <stdlib.h>

#include "kosinski_decompress.h"
#include "memory_stream.h"

size_t KosinskiDecompressModuled(const unsigned char *in_file_buffer, unsigned char **out_file_buffer)
{
	MemoryStream output_stream;
	MemoryStream_Create(&output_stream, CC_FALSE);

	const unsigned char byte1 = *in_file_buffer++;
	const size_t size = *in_file_buffer++ | (byte1 << 8);

	const unsigned char *in_file_base = in_file_buffer;

	size_t extra_module_count = ((size - 1) >> 12);
	if (extra_module_count == 0xA)
		extra_module_count = 8;

	for (size_t i = 0; i < extra_module_count; ++i)
	{
		unsigned char *out_buffer;
		size_t out_size;
		in_file_buffer += KosinskiDecompress(in_file_buffer, &out_buffer, &out_size);
		in_file_buffer += -(in_file_buffer - in_file_base) & 0xF;

		MemoryStream_Write(&output_stream, out_buffer, 1, out_size);

		free(out_buffer);
	}

	unsigned char *out_buffer;
	size_t out_size;
	KosinskiDecompress(in_file_buffer, &out_buffer, &out_size);
	MemoryStream_Write(&output_stream, out_buffer, 1, out_size);
	free(out_buffer);

	const size_t output_buffer_size = MemoryStream_GetPosition(&output_stream);
	unsigned char *output_buffer = MemoryStream_GetBuffer(&output_stream);

	MemoryStream_Destroy(&output_stream);

	if (out_file_buffer != NULL)
		*out_file_buffer = output_buffer;

	return output_buffer_size;
}
