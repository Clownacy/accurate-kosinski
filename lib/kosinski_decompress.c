// Copyright (c) 2018 Clownacy

#include "kosinski_decompress.h"

#include <stdbool.h>
#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#ifdef __MINGW32__
#define PRINTF __mingw_printf
#else
#define PRINTF printf
#endif

static unsigned short descriptor;
static unsigned int descriptor_bits_remaining;

static unsigned char *in_file_pointer;

static unsigned char *decompression_buffer;
static unsigned char *decompression_buffer_pointer;
static unsigned long decompression_buffer_size;

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

size_t KosinskiDecompress(unsigned char *in_file_buffer, unsigned char **out_file_buffer, size_t *out_file_size)
{	
	in_file_pointer = in_file_buffer;

	decompression_buffer_size = 0xA000 + 0x100;	// +0x100 to account for the copy that crossed the 0xA000 boundary
	decompression_buffer = malloc(decompression_buffer_size);
	decompression_buffer_pointer = decompression_buffer;

	GetDescriptor();

	for (;;)
	{
		if (PopDescriptor())
		{
			#ifdef DEBUG
			const unsigned long position = in_file_pointer - in_file_buffer;
			#endif

			const unsigned char byte = *in_file_pointer++;

			#ifdef DEBUG
			PRINTF("%lX - Literal match: At %zX, value %X\n", position, decompression_buffer_pointer - decompression_buffer, byte);
			#endif

			*decompression_buffer_pointer++ = byte;
		}
		else
		{
			short distance;
			unsigned int count;

			if (PopDescriptor())
			{
				#ifdef DEBUG
				const unsigned long position = in_file_pointer - in_file_buffer;
				#endif

				const unsigned char byte1 = *in_file_pointer++;
				const unsigned char byte2 = *in_file_pointer++;

				distance = byte1 | ((byte2 & 0xF8) << 5) | 0xE000;
				count = byte2 & 7;

				if (count)
				{
					count += 2;

					#ifdef DEBUG
					PRINTF("%lX - Full match: At %tX, src %tX, len %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance, count);
					#endif
				}
				else
				{
					count = *in_file_pointer++ + 1;

					if (count == 1)
					{
						#ifdef DEBUG
						PRINTF("%lX - Terminator: At %tX, src %tX\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance);
						#endif
						break;
					}
					else if (count == 2)
					{
						#ifdef DEBUG
						PRINTF("%lX - 0xA000 boundary flag: At %tX, src %tX\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance);
						#endif

						const unsigned long index = decompression_buffer_pointer - decompression_buffer;
						decompression_buffer_size += 0xA000;
						decompression_buffer = realloc(decompression_buffer, decompression_buffer_size);
						decompression_buffer_pointer = decompression_buffer + index;

						continue;
					}
					else
					{
						#ifdef DEBUG
						PRINTF("%lX - Extended full match: At %tX, src %tX, len %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance, count);
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
				const unsigned long position = in_file_pointer - in_file_buffer;
				#endif

				distance = 0xFF00 | *in_file_pointer++;

				#ifdef DEBUG
				PRINTF("%lX - Inline match: At %tX, src %tX, len %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance, count);
				#endif
			}

			unsigned char *dictionary_pointer = decompression_buffer_pointer + distance;

			for (unsigned int i = 0; i < count; ++i)
				*decompression_buffer_pointer++ = *dictionary_pointer++;
		}
	}

	if (out_file_buffer)
		*out_file_buffer = decompression_buffer;

	if (out_file_size)
		*out_file_size = decompression_buffer_pointer - decompression_buffer;

	return in_file_pointer - in_file_buffer;
}
