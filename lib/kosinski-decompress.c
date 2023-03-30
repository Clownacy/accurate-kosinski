/*
Copyright (c) 2018-2023 Clownacy

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

#include "kosinski-decompress.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int descriptor;
static unsigned int descriptor_bits_remaining;

static const unsigned char *in_file_pointer;

#define SLIDING_WINDOW_SIZE 0x2000
static unsigned char backsearch_buffer[SLIDING_WINDOW_SIZE];
static size_t write_position;

static void GetDescriptor(void)
{
	descriptor_bits_remaining = 16;

	const unsigned int low_byte = *in_file_pointer++;
	const unsigned int high_byte = *in_file_pointer++;

	descriptor = (high_byte << 8) | low_byte;
}

static bool PopDescriptor(void)
{
	const bool result = (descriptor & 1) != 0;

	descriptor >>= 1;

	if (--descriptor_bits_remaining == 0)
		GetDescriptor();

	return result;
}

static void WriteByte(const unsigned int byte, const KosinskiDecompressCallbacks* const callbacks)
{
	callbacks->write_byte((void*)callbacks->user_data, byte);
	backsearch_buffer[write_position++ % SLIDING_WINDOW_SIZE] = byte;
}

size_t KosinskiDecompress(const unsigned char *in_file_buffer, const KosinskiDecompressCallbacks *callbacks, bool print_debug_information)
{	
	in_file_pointer = in_file_buffer;

	GetDescriptor();

	for (;;)
	{
		if (PopDescriptor())
		{
			const size_t position = in_file_pointer - in_file_buffer;

			const unsigned char byte = *in_file_pointer++;

			if (print_debug_information)
				fprintf(stderr, "%zX - Literal match: At %zX, value %X\n", position, write_position, byte);

			WriteByte(byte, callbacks);
		}
		else
		{
			unsigned int distance;
			size_t count;

			if (PopDescriptor())
			{
				const size_t position = in_file_pointer - in_file_buffer;

				const unsigned char low_byte = *in_file_pointer++;
				const unsigned char high_byte = *in_file_pointer++;

				distance = 0xE000 | ((high_byte & 0xF8) << 5) | low_byte;
				distance = (distance ^ 0xFFFF) + 1; // Convert from negative two's-complement to positive
				count = high_byte & 7;

				if (count != 0)
				{
					count += 2;

					if (print_debug_information)
						fprintf(stderr, "%zX - Full match: At %zX, src %zX, len %zX\n", position, write_position, write_position - distance, count);
				}
				else
				{
					count = *in_file_pointer++ + 1;

					if (count == 1)
					{
						if (print_debug_information)
							fprintf(stderr, "%zX - Terminator: At %zX, src %zX\n", position, write_position, write_position - distance);

						break;
					}
					else if (count == 2)
					{
						if (print_debug_information)
							fprintf(stderr, "%zX - 0xA000 boundary flag: At %zX, src %zX\n", position, write_position, write_position - distance);

						continue;
					}
					else
					{
						if (print_debug_information)
							fprintf(stderr, "%zX - Extended full match: At %zX, src %zX, len %zX\n", position, write_position, write_position - distance, count);
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
					fprintf(stderr, "%zX - Inline match: At %zX, src %zX, len %zX\n", position, write_position, write_position - distance, count);
			}

			for (size_t i = 0; i < count; ++i)
				WriteByte(backsearch_buffer[(write_position - distance) % SLIDING_WINDOW_SIZE], callbacks);
		}
	}

	return in_file_pointer - in_file_buffer;
}
