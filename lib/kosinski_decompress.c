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

#include "kosinski_decompress.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory_stream.h"

static unsigned int descriptor;
static unsigned int descriptor_bits_remaining;

static const unsigned char *in_file_pointer;

static MemoryStream decompression_buffer;

static void GetDescriptor(void)
{
	descriptor_bits_remaining = 16;

	const unsigned char byte1 = *in_file_pointer++;
	const unsigned char byte2 = *in_file_pointer++;

	descriptor = (byte2 << 8) | byte1;
}

static bool PopDescriptor(void)
{
	const bool result = descriptor & 1;

	descriptor >>= 1;

	if (--descriptor_bits_remaining == 0)
		GetDescriptor();

	return result;
}

size_t KosinskiDecompress(const unsigned char *in_file_buffer, unsigned char **out_file_buffer, size_t *out_file_size, bool print_debug_information)
{	
	in_file_pointer = in_file_buffer;

	MemoryStream_Create(&decompression_buffer, CC_FALSE);

	GetDescriptor();

	for (;;)
	{
		if (PopDescriptor())
		{
			const size_t position = in_file_pointer - in_file_buffer;

			const unsigned char byte = *in_file_pointer++;

			if (print_debug_information)
				fprintf(stderr, "%zX - Literal match: At %zX, value %X\n", position, MemoryStream_GetPosition(&decompression_buffer), byte);

			MemoryStream_WriteByte(&decompression_buffer, byte);
		}
		else
		{
			unsigned int distance;
			size_t count;

			if (PopDescriptor())
			{
				const size_t position = in_file_pointer - in_file_buffer;

				const unsigned char byte1 = *in_file_pointer++;
				const unsigned char byte2 = *in_file_pointer++;

				distance = byte1 | ((byte2 & 0xF8) << 5) | 0xE000;
				distance = (distance ^ 0xFFFF) + 1; // Convert from negative two's-complement to positive
				count = byte2 & 7;

				if (count != 0)
				{
					count += 2;

					if (print_debug_information)
						fprintf(stderr, "%zX - Full match: At %zX, src %zX, len %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance, count);
				}
				else
				{
					count = *in_file_pointer++ + 1;

					if (count == 1)
					{
						if (print_debug_information)
							fprintf(stderr, "%zX - Terminator: At %zX, src %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance);

						break;
					}
					else if (count == 2)
					{
						if (print_debug_information)
							fprintf(stderr, "%zX - 0xA000 boundary flag: At %zX, src %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance);

						continue;
					}
					else
					{
						if (print_debug_information)
							fprintf(stderr, "%zX - Extended full match: At %zX, src %zX, len %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance, count);
					}
				}
			}
			else
			{
				count = 2;

				if (PopDescriptor())
					count += 2;
				if (PopDescriptor())
					count += 1;

				const size_t position = in_file_pointer - in_file_buffer;

				distance = (*in_file_pointer++ ^ 0xFF) + 1; // Convert from negative two's-complement to positive

				if (print_debug_information)
					fprintf(stderr, "%zX - Inline match: At %zX, src %zX, len %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance, count);
			}

			const size_t dictionary_index = MemoryStream_GetPosition(&decompression_buffer) - distance;

			for (size_t i = 0; i < count; ++i)
				MemoryStream_WriteByte(&decompression_buffer, MemoryStream_GetBuffer(&decompression_buffer)[dictionary_index + i]);
		}
	}

	if (out_file_buffer != NULL)
		*out_file_buffer = MemoryStream_GetBuffer(&decompression_buffer);

	if (out_file_size != NULL)
		*out_file_size = MemoryStream_GetPosition(&decompression_buffer);

	MemoryStream_Destroy(&decompression_buffer);

	return in_file_pointer - in_file_buffer;
}
