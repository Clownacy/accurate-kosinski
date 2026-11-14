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

#include "lib/kosinski-compress.h"

static unsigned int ReadByte(void* const user_data)
{
	const int byte = fgetc((FILE*)user_data);

	return byte == EOF ? -1 : byte;
}

static void WriteByte(void* const user_data, const unsigned int byte)
{
	fputc(byte, (FILE*)user_data);
}

int main(const int argc, char** const argv)
{
	int exit_code = EXIT_SUCCESS;
	FILE *in_file = stdin;
	FILE *out_file = stdout;

	if (argc > 2) {
	#ifdef DEBUG
		unsigned long file_size;
		fseek(in_file, 0, SEEK_END);
		file_size = ftell(in_file);
		rewind(in_file);

		fprintf(stderr, "File '%s' with size %lX loaded\n", argv[1], file_size);
	#endif

		out_file = fopen(argv[2], "wb");

		if (out_file == NULL)
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[2]);
		}
	}

	if (argc > 1) {
		in_file = fopen(argv[1], "rb");

		if (in_file == NULL)
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
	}

	KosinskiCompressCallbacks callbacks;
	callbacks.read_byte_user_data = in_file;
	callbacks.read_byte = ReadByte;
	callbacks.write_byte_user_data = out_file;
	callbacks.write_byte = WriteByte;

	KosinskiCompress(&callbacks,
	#ifdef DEBUG
		cc_true
	#else
		cc_false
	#endif
	);

	if (argc > 2)
		fclose(out_file);
	
	if (argc > 1)
		fclose(in_file);

	return exit_code;
}
