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

#include "kosinski-moduled-decompress.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "kosinski-decompress.h"

#define MODULE_SIZE 0x1000

size_t KosinskiDecompressModuled(const unsigned char *in_file_buffer, void (*write_byte)(void *user_data, unsigned int byte), const void *user_data, bool print_debug_messages)
{
	const unsigned int raw_size = ((unsigned int)in_file_buffer[0] << 8) | in_file_buffer[1];
	const unsigned int size = raw_size == 0xA000 ? 0x8000 : raw_size;

	const unsigned char *in_file_pointer = in_file_buffer + 2;

	for (unsigned int i = 0; i < size; i += MODULE_SIZE)
	{
		const size_t bytes_read = KosinskiDecompress(in_file_pointer, write_byte, user_data, print_debug_messages);
		in_file_pointer += bytes_read + ((0 - bytes_read) % 0x10);
	}

	return in_file_pointer - in_file_buffer;
}
