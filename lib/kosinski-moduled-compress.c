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

#include <stddef.h>
#include <stdlib.h>

#define MODULE_SIZE 0x1000

typedef struct CallbacksAndCounter
{
	const KosinskiCompressCallbacks *callbacks;
	unsigned int bytes_remaining;
} CallbacksAndCounter;

static unsigned int ReadByte(void* const user_data)
{
	CallbacksAndCounter* const callbacks_and_counter = (CallbacksAndCounter*)user_data;

	if (callbacks_and_counter->bytes_remaining == 0)
		return -1;

	--callbacks_and_counter->bytes_remaining;

	return callbacks_and_counter->callbacks->read_byte((void*)callbacks_and_counter->callbacks->read_byte_user_data);
}

void KosinskiCompressModuled(const size_t file_size, const KosinskiCompressCallbacks* const callbacks, const cc_bool print_debug_messages)
{
	size_t file_index;

	if (file_size > 0xFFFF || file_size == 0xA000) /* For some reason, 0xA000 is forced to 0x8000 in Sonic 3 & Knuckles' `Process_Kos_Module_Queue_Init` function. */
		return;	/* Cannot fit size of file in header - give up */ /* TODO: Error code? */

	callbacks->write_byte((void*)callbacks->write_byte_user_data, file_size >> 8);
	callbacks->write_byte((void*)callbacks->write_byte_user_data, file_size & 0xFF);

	CallbacksAndCounter callbacks_and_counter;
	callbacks_and_counter.callbacks = callbacks;

	KosinskiCompressCallbacks new_callbacks;
	new_callbacks.read_byte_user_data = &callbacks_and_counter;
	new_callbacks.read_byte = ReadByte;
	new_callbacks.write_byte_user_data = callbacks->write_byte_user_data;
	new_callbacks.write_byte = callbacks->write_byte;

	for (file_index = 0; file_index < file_size; file_index += MODULE_SIZE)
	{
		callbacks_and_counter.bytes_remaining = MODULE_SIZE;
		KosinskiCompress(&new_callbacks, print_debug_messages);
	}
}
