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

#define SLIDING_WINDOW_SIZE 0x2000
static unsigned char backsearch_buffer[SLIDING_WINDOW_SIZE];
static size_t read_position;
static size_t write_position;

static unsigned int ReadByte(const KosinskiDecompressCallbacks* const callbacks)
{
	++read_position;
	return callbacks->read_byte((void*)callbacks->read_byte_user_data);
}

static void WriteByte(const unsigned int byte, const KosinskiDecompressCallbacks* const callbacks)
{
	callbacks->write_byte((void*)callbacks->write_byte_user_data, byte);
	backsearch_buffer[write_position++ % SLIDING_WINDOW_SIZE] = byte;
}


static void GetDescriptor(const KosinskiDecompressCallbacks* const callbacks)
{
	descriptor_bits_remaining = 16;

	const unsigned int low_byte = ReadByte(callbacks);
	const unsigned int high_byte = ReadByte(callbacks);

	descriptor = (high_byte << 8) | low_byte;
}

static bool PopDescriptor(const KosinskiDecompressCallbacks* const callbacks)
{
	const bool result = (descriptor & 1) != 0;

	descriptor >>= 1;

	if (--descriptor_bits_remaining == 0)
		GetDescriptor(callbacks);

	return result;
}

void KosinskiDecompress(const KosinskiDecompressCallbacks *callbacks, bool print_debug_information)
{
	read_position = 0;
	write_position = 0;

	GetDescriptor(callbacks);

	for (;;)
	{
		if (PopDescriptor(callbacks))
		{
			const size_t position = read_position;

			const unsigned char byte = ReadByte(callbacks);

			if (print_debug_information)
				fprintf(stderr, "%zX - Literal match: At %zX, value %X\n", position, write_position, byte);

			WriteByte(byte, callbacks);
		}
		else
		{
			unsigned int distance;
			size_t count;

			if (PopDescriptor(callbacks))
			{
				const size_t position = read_position;

				const unsigned char low_byte = ReadByte(callbacks);
				const unsigned char high_byte = ReadByte(callbacks);

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
					count = ReadByte(callbacks) + 1;

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

				if (PopDescriptor(callbacks))
					count += 2;
				if (PopDescriptor(callbacks))
					count += 1;

				const size_t position = read_position;

				distance = (ReadByte(callbacks) ^ 0xFF) + 1; // Convert from negative two's-complement to positive

				if (print_debug_information)
					fprintf(stderr, "%zX - Inline match: At %zX, src %zX, len %zX\n", position, write_position, write_position - distance, count);
			}

			for (size_t i = 0; i < count; ++i)
				WriteByte(backsearch_buffer[(write_position - distance) % SLIDING_WINDOW_SIZE], callbacks);
		}
	}
}
