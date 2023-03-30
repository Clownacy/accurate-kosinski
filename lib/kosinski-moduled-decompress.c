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

typedef struct CallbacksAndCounter
{
	const KosinskiDecompressCallbacks *callbacks;
	size_t read_position;
} CallbacksAndCounter;

static unsigned int ReadByte(void* const user_data)
{
	CallbacksAndCounter* const callbacks_and_counter = (CallbacksAndCounter*)user_data;

	++callbacks_and_counter->read_position;

	return callbacks_and_counter->callbacks->read_byte((void*)callbacks_and_counter->callbacks->read_byte_user_data);
}

void KosinskiDecompressModuled(const KosinskiDecompressCallbacks *callbacks, bool print_debug_messages)
{
	const unsigned int high_byte = callbacks->read_byte((void*)callbacks->read_byte_user_data);
	const unsigned int low_byte = callbacks->read_byte((void*)callbacks->read_byte_user_data);
	const unsigned int raw_size = (high_byte << 8) | low_byte;
	const unsigned int size = raw_size == 0xA000 ? 0x8000 : raw_size;

	CallbacksAndCounter callbacks_and_counter;
	callbacks_and_counter.callbacks = callbacks;
	callbacks_and_counter.read_position = 0;

	KosinskiDecompressCallbacks new_callbacks;
	new_callbacks.read_byte_user_data = &callbacks_and_counter;
	new_callbacks.read_byte = ReadByte;
	new_callbacks.write_byte_user_data = callbacks->write_byte_user_data;
	new_callbacks.write_byte = callbacks->write_byte;

	for (unsigned int i = 0; i < size; i += MODULE_SIZE)
	{
		KosinskiDecompress(&new_callbacks, print_debug_messages);

		const unsigned int padding_bytes = (0 - callbacks_and_counter.read_position) % 0x10;

		for (unsigned int j = 0; j < padding_bytes; ++j)
			new_callbacks.read_byte((void*)new_callbacks.read_byte_user_data);
	}
}
