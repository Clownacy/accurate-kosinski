/*
 * zlib License
 *
 * (C) 2018-2021 Clownacy
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
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
