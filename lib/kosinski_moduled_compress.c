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

#include "kosinski_moduled_compress.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "kosinski_compress.h"
#include "memory_stream.h"

void KosinskiCompressModuled(const unsigned char *file_buffer, size_t file_size, void (*write_byte)(void *user_data, unsigned int byte), const void *user_data, bool print_debug_messages)
{
	if (file_size > 0xFFFF)
		return;	// Cannot fit size of file in header - give up // TODO: Error code?

	write_byte((void*)user_data, (file_size >> 8) & 0xFF);
	write_byte((void*)user_data, (file_size >> 0) & 0xFF);

	const size_t extra_module_count = (file_size - 1) >> 12;

	for (size_t i = 0; i < extra_module_count; ++i)
	{
		KosinskiCompress(file_buffer, 0x1000, write_byte, user_data, print_debug_messages);
		file_buffer += 0x1000;
	}

	KosinskiCompress(file_buffer, ((file_size - 1) & 0xFFF) + 1, write_byte, user_data, print_debug_messages);
}
