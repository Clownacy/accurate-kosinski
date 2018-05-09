#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short descriptor;

FILE *file;
FILE *out_file;

unsigned char backsearch_buffer[0x2000];
size_t backsearch_buffer_index;

bool PopDescriptor(void)
{
	static int bits_remaining = 16;

	bool result = descriptor & 1;

	descriptor >>= 1;

	if (--bits_remaining == 0)
	{
		fread(&descriptor, 2, 1, file);
		bits_remaining = 16;
	}

	return result;
}

unsigned char GetByte(void)
{
	int result = fgetc(file);

	return result;
}

void WriteBytes(unsigned int distance, unsigned int count)
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

	unsigned int decomp_pointer = 0;
	fread(&descriptor, 2, 1, file);

	for (;;)
	{
		if (PopDescriptor())
		{
			size_t position = ftell(file);
			unsigned char byte = GetByte();
			#ifndef SHUTUP
			printf("%X - Literal match: At %X, value %X\n", position, decomp_pointer, byte);
			#endif
			++decomp_pointer;

			fputc(byte, out_file);
			backsearch_buffer[backsearch_buffer_index++ & 0x1FFF] = byte;
		}
		else if (PopDescriptor())
		{
			size_t position = ftell(file);
			unsigned char byte1 = GetByte();
			unsigned char byte2 = GetByte();
			short distance = byte1 | ((byte2 & 0xF8) << 5) | 0xE000;
			unsigned char count = byte2 & 7;
			if (count)
			{
				count += 2;
				#ifndef SHUTUP
				printf("%X - Full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
				#endif
			}
			else
			{
				count = GetByte() + 1;
				if (count == 1)
				{
					#ifndef SHUTUP
					printf("%X - Terminator: At %X, src %X\n", position, decomp_pointer, decomp_pointer + distance);
					#endif
					break;
				}
				else if (count < 10)
				{
					#ifndef SHUTUP
					printf("%X - Potential dummy terminator: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
					#endif
				}
				else
				{
					#ifndef SHUTUP
					printf("%X - Extended full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
					#endif
				}
			}
			decomp_pointer += count;

			WriteBytes(distance, count);
		}
		else
		{
			size_t position = ftell(file);
			int count = 2;

			if (PopDescriptor())
				count += 2;
			if (PopDescriptor())
				count += 1;

			short distance = 0xFF00 | GetByte();
			#ifndef SHUTUP
			printf("%X - Inline match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
			#endif
			decomp_pointer += count;

			WriteBytes(distance, count);
		}
	}
}
