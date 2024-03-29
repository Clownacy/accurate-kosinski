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

#ifndef KOSINSKI_DECOMPRESS_H
#define KOSINSKI_DECOMPRESS_H

#include <stddef.h>

#include "clowncommon/clowncommon.h"

typedef struct KosinskiDecompressCallbacks
{
	const void *read_byte_user_data;
	unsigned int (*read_byte)(void *user_data);
	const void *write_byte_user_data;
	void (*write_byte)(void *user_data, unsigned int byte);
} KosinskiDecompressCallbacks;

void KosinskiDecompress(const KosinskiDecompressCallbacks *callbacks, cc_bool print_debug_information);

#endif /* KOSINSKI_DECOMPRESS_H */
