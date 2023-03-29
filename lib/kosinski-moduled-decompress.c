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

size_t KosinskiDecompressModuled(const unsigned char *in_file_buffer, void (*write_byte)(void *user_data, unsigned int byte), const void *user_data, bool print_debug_messages)
{
	const unsigned char *in_file_pointer = in_file_buffer;

	const unsigned char byte1 = *in_file_pointer++;
	const size_t size = *in_file_pointer++ | (byte1 << 8);

	const unsigned char *in_file_base = in_file_pointer;

	size_t extra_module_count = ((size - 1) >> 12);
	if (extra_module_count == 0xA)
		extra_module_count = 8;

	for (size_t i = 0; i < extra_module_count; ++i)
	{
		in_file_pointer += KosinskiDecompress(in_file_pointer, write_byte, user_data, print_debug_messages);
		in_file_pointer += -(in_file_pointer - in_file_base) & 0xF;
	}

	KosinskiDecompress(in_file_buffer, write_byte, user_data, print_debug_messages);

	return in_file_pointer - in_file_buffer;
}
