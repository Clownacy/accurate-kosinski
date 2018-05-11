#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned short descriptor;
static int descriptor_bits_remaining;

static FILE *file;
static FILE *out_file;

static unsigned char backsearch_buffer[0x2000];
static size_t backsearch_buffer_index;

static bool PopDescriptor(void)
{
	bool result = descriptor & 1;

	descriptor >>= 1;

	if (--descriptor_bits_remaining == 0)
	{
		fread(&descriptor, 2, 1, file);
		descriptor_bits_remaining = 16;
	}

	return result;
}

static void WriteBytes(unsigned int distance, unsigned int count)
{
	for (unsigned int i = 0; i < count; ++i)
	{
		unsigned char byte = backsearch_buffer[(backsearch_buffer_index + distance) & 0x1FFF];

		fputc(byte, out_file);
		backsearch_buffer[backsearch_buffer_index & 0x1FFF] = byte;

		++backsearch_buffer_index;
	}
}

void KosinskiDecompress(FILE *p_in_file, FILE *p_out_file)
{	
	file = p_in_file;
	out_file = p_out_file;
	descriptor_bits_remaining = 16;

	unsigned int decomp_pointer = 0;
	fread(&descriptor, 2, 1, file);

	for (;;)
	{
		if (PopDescriptor())
		{
			unsigned char byte = fgetc(file);

			#ifndef SHUTUP
			long int position = ftell(file);
			printf("%lX - Literal match: At %X, value %X\n", position, decomp_pointer, byte);
			#endif

			fputc(byte, out_file);
			backsearch_buffer[backsearch_buffer_index++ & 0x1FFF] = byte;

			++decomp_pointer;
		}
		else if (PopDescriptor())
		{
			#ifndef SHUTUP
			long int position = ftell(file);
			#endif

			unsigned char byte1 = fgetc(file);
			unsigned char byte2 = fgetc(file);

			short distance = byte1 | ((byte2 & 0xF8) << 5) | 0xE000;
			unsigned char count = byte2 & 7;

			if (count)
			{
				count += 2;

				#ifndef SHUTUP
				printf("%lX - Full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
				#endif
			}
			else
			{
				count = fgetc(file) + 1;

				if (count == 1)
				{
					#ifndef SHUTUP
					printf("%lX - Terminator: At %X, src %X\n", position, decomp_pointer, decomp_pointer + distance);
					#endif
					break;
				}
				else if (count == 2)
				{
					#ifndef SHUTUP
					printf("%lX - Dummy terminator: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
					#endif
					continue;
				}
				else
				{
					#ifndef SHUTUP
					printf("%lX - Extended full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
					#endif
				}
			}

			WriteBytes(distance, count);

			decomp_pointer += count;
		}
		else
		{
			#ifndef SHUTUP
			long int position = ftell(file);
			#endif

			unsigned int count = 2;

			if (PopDescriptor())
				count += 2;
			if (PopDescriptor())
				count += 1;

			short distance = 0xFF00 | fgetc(file);

			#ifndef SHUTUP
			printf("%lX - Inline match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
			#endif

			WriteBytes(distance, count);

			decomp_pointer += count;
		}
	}
}
