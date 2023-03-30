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

#include "kosinski-moduled-compress.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "kosinski-compress.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MODULE_SIZE 0x1000

void KosinskiCompressModuled(const unsigned char *file_buffer, size_t file_size, const KosinskiCompressCallbacks *callbacks, bool print_debug_messages)
{
	if (file_size > 0xFFFF || file_size == 0xA000) // For some reason, 0xA000 is forced to 0x8000 in Sonic 3 & Knuckles' `Process_Kos_Module_Queue_Init` function.
		return;	// Cannot fit size of file in header - give up // TODO: Error code?

	callbacks->write_byte((void*)callbacks->user_data, file_size >> 8);
	callbacks->write_byte((void*)callbacks->user_data, file_size & 0xFF);

	for (size_t file_index = 0; file_index < file_size; file_index += MODULE_SIZE)
		KosinskiCompress(&file_buffer[file_index], MIN(MODULE_SIZE, file_size - file_index), callbacks, print_debug_messages);
}
