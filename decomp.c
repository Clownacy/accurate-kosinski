#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned short descriptor;

FILE *file;

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

	if (result == EOF)
		exit(EXIT_SUCCESS);

	return result;
}

int main(int argc, char *argv[])
{
	file = fopen(argv[1], "rb");
	unsigned int decomp_pointer = 0;
	fread(&descriptor, 2, 1, file);

	for (;;)
	{
		if (PopDescriptor())
		{
			size_t position = ftell(file);
			printf("%X - Literal match: At %X, value %X\n", position, decomp_pointer, GetByte());
			++decomp_pointer;
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
				printf("%X - Full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
			}
			else
			{
				count = GetByte() + 1;
				if (count == 1)
				{
					printf("%X - Terminator: At %X, src %X\n", position, decomp_pointer, decomp_pointer + distance);
					exit(EXIT_SUCCESS);
				}
				else if (count < 10)
				{
					printf("%X - Potential dummy terminator: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
				}
				else
				{
					printf("%X - Extended full match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
				}
			}
			decomp_pointer += count;
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
			printf("%X - Inline match: At %X, src %X, len %X\n", position, decomp_pointer, decomp_pointer + distance, count);
			decomp_pointer += count;
		}
	}
}
