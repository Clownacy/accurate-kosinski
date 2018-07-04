// Copyright (c) 2018 Clownacy

#include "kosinski_decompress.h"

#include <stdbool.h>
#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

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

static void WriteBytes(short distance, unsigned int count)
{
	unsigned char *dictionary_pointer = decompression_buffer_pointer + distance;

	for (unsigned int i = 0; i < count; ++i)
		*decompression_buffer_pointer++ = *dictionary_pointer++;
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
			printf("%lX - Literal match: At %X, value %X\n", position, decompression_buffer_pointer - decompression_buffer, byte);
			#endif

			*decompression_buffer_pointer++ = byte;
		}
		else if (PopDescriptor())
		{
			#ifdef DEBUG
			const unsigned long position = in_file_pointer - in_file_buffer;
			#endif

			const unsigned char byte1 = *in_file_pointer++;
			const unsigned char byte2 = *in_file_pointer++;

			const short distance = byte1 | ((byte2 & 0xF8) << 5) | 0xE000;
			unsigned char count = byte2 & 7;

			if (count)
			{
				count += 2;

				#ifdef DEBUG
				printf("%lX - Full match: At %X, src %X, len %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance, count);
				#endif
			}
			else
			{
				count = *in_file_pointer++ + 1;

				if (count == 1)
				{
					#ifdef DEBUG
					printf("%lX - Terminator: At %X, src %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance);
					#endif
					break;
				}
				else if (count == 2)
				{
					#ifdef DEBUG
					printf("%lX - 0xA000 boundary flag: At %X, src %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance);
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
					printf("%lX - Extended full match: At %X, src %X, len %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance, count);
					#endif
				}
			}

			WriteBytes(distance, count);
		}
		else
		{
			unsigned int count = 2;

			if (PopDescriptor())
				count += 2;
			if (PopDescriptor())
				count += 1;

			#ifdef DEBUG
			const unsigned long position = in_file_pointer - in_file_buffer;
			#endif

			const short distance = 0xFF00 | *in_file_pointer++;

			#ifdef DEBUG
			printf("%lX - Inline match: At %X, src %X, len %X\n", position, decompression_buffer_pointer - decompression_buffer, decompression_buffer_pointer - decompression_buffer + distance, count);
			#endif

			WriteBytes(distance, count);
		}
	}

	if (out_file_buffer)
		*out_file_buffer = decompression_buffer;

	if (out_file_size)
		*out_file_size = decompression_buffer_pointer - decompression_buffer;

	return in_file_pointer - in_file_buffer;
}
