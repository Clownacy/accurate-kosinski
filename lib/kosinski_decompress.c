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

#include "kosinski_decompress.h"

#include <stdbool.h>
#include <stddef.h>
#ifdef DEBUG
 #include <stdio.h>
#endif
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

EXPORTS size_t KosinskiDecompress(const unsigned char *in_file_buffer, unsigned char **out_file_buffer, size_t *out_file_size)
{	
	in_file_pointer = in_file_buffer;

	MemoryStream_Create(&decompression_buffer, CC_FALSE);

	GetDescriptor();

	for (;;)
	{
		if (PopDescriptor())
		{
		#ifdef DEBUG
			const size_t position = in_file_pointer - in_file_buffer;
		#endif

			const unsigned char byte = *in_file_pointer++;

		#ifdef DEBUG
			fprintf(stderr, "%zX - Literal match: At %zX, value %X\n", position, MemoryStream_GetPosition(&decompression_buffer), byte);
		#endif

			MemoryStream_WriteByte(&decompression_buffer, byte);
		}
		else
		{
			unsigned int distance;
			size_t count;

			if (PopDescriptor())
			{
			#ifdef DEBUG
				const size_t position = in_file_pointer - in_file_buffer;
			#endif

				const unsigned char byte1 = *in_file_pointer++;
				const unsigned char byte2 = *in_file_pointer++;

				distance = byte1 | ((byte2 & 0xF8) << 5) | 0xE000;
				distance = (distance ^ 0xFFFF) + 1; // Convert from negative two's-complement to positive
				count = byte2 & 7;

				if (count != 0)
				{
					count += 2;

				#ifdef DEBUG
					fprintf(stderr, "%zX - Full match: At %zX, src %zX, len %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance, count);
				#endif
				}
				else
				{
					count = *in_file_pointer++ + 1;

					if (count == 1)
					{
					#ifdef DEBUG
						fprintf(stderr, "%zX - Terminator: At %zX, src %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance);
					#endif
						break;
					}
					else if (count == 2)
					{
					#ifdef DEBUG
						fprintf(stderr, "%zX - 0xA000 boundary flag: At %zX, src %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance);
					#endif
						continue;
					}
					else
					{
					#ifdef DEBUG
						fprintf(stderr, "%zX - Extended full match: At %zX, src %zX, len %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance, count);
					#endif
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

			#ifdef DEBUG
				const size_t position = in_file_pointer - in_file_buffer;
			#endif

				distance = (*in_file_pointer++ ^ 0xFF) + 1; // Convert from negative two's-complement to positive

			#ifdef DEBUG
				fprintf(stderr, "%zX - Inline match: At %zX, src %zX, len %zX\n", position, MemoryStream_GetPosition(&decompression_buffer), MemoryStream_GetPosition(&decompression_buffer) - distance, count);
			#endif
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
