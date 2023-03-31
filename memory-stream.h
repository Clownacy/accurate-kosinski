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

#ifndef MEMORY_STREAM_H
#define MEMORY_STREAM_H

#include <stddef.h>

#include "clowncommon/clowncommon.h"

typedef struct MemoryStream
{
	unsigned char *buffer;
	size_t position;
	size_t end;
	size_t size;
	cc_bool free_buffer_when_destroyed;
} MemoryStream;

typedef struct ROMemoryStream
{
	MemoryStream memory_stream;
} ROMemoryStream;

enum MemoryStream_Origin
{
	MEMORYSTREAM_START,
	MEMORYSTREAM_CURRENT,
	MEMORYSTREAM_END
};

#ifdef __cplusplus
extern "C" {
#endif

void MemoryStream_Create(MemoryStream *memory_stream, cc_bool free_buffer_when_destroyed);
void MemoryStream_Destroy(MemoryStream *memory_stream);
cc_bool MemoryStream_WriteByte(MemoryStream *memory_stream, unsigned int byte);
cc_bool MemoryStream_Write(MemoryStream *memory_stream, const void *data, size_t size, size_t count);
size_t MemoryStream_Read(MemoryStream *memory_stream, void *output, size_t size, size_t count);
unsigned char* MemoryStream_GetBuffer(MemoryStream *memory_stream);
size_t MemoryStream_GetPosition(MemoryStream *memory_stream);
cc_bool MemoryStream_SetPosition(MemoryStream *memory_stream, ptrdiff_t offset, enum MemoryStream_Origin origin);
void MemoryStream_Rewind(MemoryStream *memory_stream);

void ROMemoryStream_Create(ROMemoryStream *ro_memory_stream, const void *data, size_t size);
void ROMemoryStream_Destroy(ROMemoryStream *ro_memory_stream);
size_t ROMemoryStream_Read(ROMemoryStream *ro_memory_stream, void *output, size_t size, size_t count);
size_t ROMemoryStream_GetPosition(ROMemoryStream *ro_memory_stream);
cc_bool ROMemoryStream_SetPosition(ROMemoryStream *ro_memory_stream, ptrdiff_t offset, enum MemoryStream_Origin origin);
void ROMemoryStream_Rewind(ROMemoryStream *ro_memory_stream);

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_STREAM_H */
