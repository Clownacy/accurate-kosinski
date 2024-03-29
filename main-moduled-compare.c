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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/kosinski-moduled-compress.h"
#include "lib/kosinski-moduled-decompress.h"

#include "load-file-to-buffer.h"
#include "memory-stream.h"

static unsigned int ReadByte(void* const user_data)
{
	unsigned char byte;
	return ROMemoryStream_Read((ROMemoryStream*)user_data, &byte, 1, 1) == 0 ? -1 : byte;
}

static void WriteByte(void* const user_data, const unsigned int byte)
{
	MemoryStream_WriteByte((MemoryStream*)user_data, byte);
}

int main(const int argc, char** const argv)
{
	int i;

	int exit_code = EXIT_SUCCESS;

	for (i = 1; i < argc; ++i)
	{
		unsigned char *in_file_buffer;
		size_t in_file_size;
		if (!LoadFileToBuffer(argv[i], &in_file_buffer, &in_file_size))
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[i]);
		}
		else
		{
			ROMemoryStream compressed_buffer;
			MemoryStream uncompressed_buffer;
			KosinskiDecompressCallbacks decompress_callbacks;
			ROMemoryStream uncompressed_buffer_read;
			MemoryStream recompressed_buffer;
			KosinskiCompressCallbacks compress_callbacks;

			ROMemoryStream_Create(&compressed_buffer, in_file_buffer, in_file_size);
			MemoryStream_Create(&uncompressed_buffer, cc_true);

			decompress_callbacks.read_byte_user_data = &compressed_buffer;
			decompress_callbacks.read_byte = ReadByte;
			decompress_callbacks.write_byte_user_data = &uncompressed_buffer;
			decompress_callbacks.write_byte = WriteByte;

			KosinskiDecompressModuled(&decompress_callbacks,
			#ifdef DEBUG
				cc_true
			#else
				cc_false
			#endif
			);

			ROMemoryStream_Destroy(&compressed_buffer);

		#ifdef DEBUG
			fprintf(stderr, "File '%s' with size %lX loaded\n", argv[i], (unsigned long)MemoryStream_GetPosition(&uncompressed_buffer));
		#endif

			ROMemoryStream_Create(&uncompressed_buffer_read, MemoryStream_GetBuffer(&uncompressed_buffer), MemoryStream_GetPosition(&uncompressed_buffer));
			MemoryStream_Create(&recompressed_buffer, cc_true);

			compress_callbacks.read_byte_user_data = &uncompressed_buffer_read;
			compress_callbacks.read_byte = ReadByte;
			compress_callbacks.write_byte_user_data = &recompressed_buffer;
			compress_callbacks.write_byte = WriteByte;

			KosinskiCompressModuled(MemoryStream_GetPosition(&uncompressed_buffer), &compress_callbacks,
			#ifdef DEBUG
				cc_true
			#else
				cc_false
			#endif
			);

			ROMemoryStream_Destroy(&uncompressed_buffer_read);
			MemoryStream_Destroy(&uncompressed_buffer);

			if (in_file_size != MemoryStream_GetPosition(&recompressed_buffer))
			{
				exit_code = EXIT_FAILURE;
				fputs("FAILURE: The size of the recompressed data does not match the original.\n", stdout);
			}
			else if (memcmp(in_file_buffer, MemoryStream_GetBuffer(&recompressed_buffer), CC_MIN(in_file_size, MemoryStream_GetPosition(&recompressed_buffer))))
			{
				exit_code = EXIT_FAILURE;
				fputs("FAILURE: The recompressed data does not match the original.\n", stdout);
			}
			else
			{
				fputs("SUCCESS: The recompressed data matches the original.\n", stdout);
			}

			MemoryStream_Destroy(&recompressed_buffer);
			free(in_file_buffer);
		}
	}

	return exit_code;
}
