#include "kosinski_decompress.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory_stream.h"

static unsigned short descriptor;
static int descriptor_bits_remaining;

static unsigned char *in_file_pointer;
MemoryStream *output_stream;

static unsigned char backsearch_buffer[0x2000];
static size_t backsearch_buffer_index;

static void GetDescriptor(void)
{
	descriptor_bits_remaining = 16;

	descriptor = (in_file_pointer[1] << 8) | in_file_pointer[0];
	in_file_pointer += 2;
}

static bool PopDescriptor(void)
{
	bool result = descriptor & 1;

	descriptor >>= 1;

	if (--descriptor_bits_remaining == 0)
	{
		GetDescriptor();
	}

	return result;
}

static void WriteBytes(unsigned int distance, unsigned int count)
{
	for (unsigned int i = 0; i < count; ++i)
	{
		unsigned char byte = backsearch_buffer[(backsearch_buffer_index + distance) & 0x1FFF];

		MemoryStream_WriteByte(output_stream, byte);
		backsearch_buffer[backsearch_buffer_index & 0x1FFF] = byte;

		++backsearch_buffer_index;
	}
}

size_t KosinskiDecompress(unsigned char *in_file_buffer, unsigned char **out_file_buffer, size_t *out_file_size)
{	
	in_file_pointer = in_file_buffer;

	output_stream = MemoryStream_Init(0x100);

	unsigned int decomp_pointer = 0;
	GetDescriptor();

	for (;;)
	{
		if (PopDescriptor())
		{
			#ifdef DEBUG
			long int position = in_file_pointer - in_file_buffer;
			#endif

			unsigned char byte = *in_file_pointer++;

			#ifdef DEBUG
			printf("%lX - Literal match: At %X, value %X\n", position, decomp_pointer, byte);
			#endif

			MemoryStream_WriteByte(output_stream, byte);
			backsearch_buffer[backsearch_buffer_index++ & 0x1FFF] = byte;

			++decomp_pointer;
		}
		else if (PopDescriptor())
		{
			#ifdef DEBUG
			long int position = in_file_pointer - in_file_buffer;
			#endif

			unsigned char byte1 = *in_file_pointer++;
			unsigned char byte2 = *in_file_pointer++;

			short distance = byte1 | ((byte2 & 0xF8) << 5) | 0xE000;
			unsigned char count = byte2 & 7;

			if (count)
			{
				count += 2;

				#ifdef DEBUG
				printf("%lX - Full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
				#endif
			}
			else
			{
				count = *in_file_pointer++ + 1;

				if (count == 1)
				{
					#ifdef DEBUG
					printf("%lX - Terminator: At %X, src %X\n", position, decomp_pointer, decomp_pointer + distance);
					#endif
					break;
				}
				else if (count == 2)
				{
					#ifdef DEBUG
					printf("%lX - Dummy terminator: At %X, src %X\n", position, decomp_pointer, decomp_pointer + distance);
					#endif
					continue;
				}
				else
				{
					#ifdef DEBUG
					printf("%lX - Extended full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
					#endif
				}
			}

			WriteBytes(distance, count);

			decomp_pointer += count;
		}
		else
		{
			unsigned int count = 2;

			if (PopDescriptor())
				count += 2;
			if (PopDescriptor())
				count += 1;

			#ifdef DEBUG
			long int position = in_file_pointer - in_file_buffer;
			#endif

			short distance = 0xFF00 | *in_file_pointer++;

			#ifdef DEBUG
			printf("%lX - Inline match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
			#endif

			WriteBytes(distance, count);

			decomp_pointer += count;
		}
	}

	size_t output_buffer_size = MemoryStream_GetIndex(output_stream);
	unsigned char *output_buffer = MemoryStream_GetBuffer(output_stream);

	free(output_stream);

	if (out_file_buffer)
		*out_file_buffer = output_buffer;

	if (out_file_size)
		*out_file_size = output_buffer_size;

	return in_file_pointer - in_file_buffer;
}
