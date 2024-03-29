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

#include "memory-stream.h"

static MemoryStream memory_stream;

static unsigned int ReadByte(void* const user_data)
{
	const int byte = fgetc((FILE*)user_data);

	return byte == EOF ? -1 : byte;
}

static void WriteByte(void* const user_data, const unsigned int byte)
{
	MemoryStream_WriteByte((MemoryStream*)user_data, byte);
}

int main(const int argc, char** const argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		fputs(
			"This tool compresses a supplied file in the Kosinski format. It tries to produce files accurate to Sega's original compressor.\n"
			"This particular tool outputs an assembly file, matching the format of the one found in Sonic 2's 'Simon Wai' prototype.\n"
			"\n"
			"www.github.com/Clownacy/accurate-kosinski\n"
			"\n"
			"Usage: kosinski-compress-asm [in_file] [out_file](optional)\n"
			, stdout
		);
	}
	else
	{
		FILE* const in_file = fopen(argv[1], "rb");

		if (in_file == NULL)
		{
			exit_code = EXIT_FAILURE;
			fprintf(stderr, "Could not open '%s'\n", argv[1]);
		}
		else
		{
			unsigned long file_size;
			KosinskiCompressCallbacks callbacks;
			const char *out_filename;
			FILE *out_file;

			fseek(in_file, 0, SEEK_END);
			file_size = ftell(in_file);
			rewind(in_file);

		#ifdef DEBUG
			fprintf(stderr, "File '%s' with size %lX loaded\n", argv[1], file_size);
		#endif

			MemoryStream_Create(&memory_stream, cc_true);

			callbacks.read_byte_user_data = in_file;
			callbacks.read_byte = ReadByte;
			callbacks.write_byte_user_data = &memory_stream;
			callbacks.write_byte = WriteByte;

			KosinskiCompress(&callbacks,
			#ifdef DEBUG
				cc_true
			#else
				cc_false
			#endif
			);

			fclose(in_file);

			out_filename = (argc > 2) ? argv[2] : "out.asm";
			out_file = fopen(out_filename, "w");

			if (out_file == NULL)
			{
				exit_code = EXIT_FAILURE;
				fprintf(stderr, "Could not open '%s'\n", out_filename);
			}
			else
			{
				size_t i;

				const unsigned char *out_pointer = MemoryStream_GetBuffer(&memory_stream);
				const size_t out_size = MemoryStream_GetPosition(&memory_stream);
				const size_t claimed_out_size = out_size + ((0 - out_size) % 0x100);

				/* Shift-JIS: Supposedly translates to 'Before compression', 'After compression', 'Compression ratio', and 'Number of cells'.
				   A 'cell' is what we call a 'tile'. This suggests that Kosinski was intended for compressing tiles, rather than any other kind of data. */
				fprintf(out_file, "; \x88\xB3\x8F\x6B\x91\x4F $%lx  \x88\xB3\x8F\x6B\x8C\xE3 $%lx  \x88\xB3\x8F\x6B\x97\xA6 %.1f%%  \x83\x5A\x83\x8B\x90\x94 %ld\n", file_size, (unsigned long)claimed_out_size, ((float)claimed_out_size / file_size) * 100.0f, file_size / (8 * 8 / 2));

				for (i = 0; i < out_size; i += 0x10)
				{
					unsigned int j;

					fprintf(out_file, "	dc.b	$%.2x", *out_pointer++);

					j = 1;

					for (; j < CC_MIN(0x10, out_size - i); ++j)
						fprintf(out_file, ",$%.2x", *out_pointer++);

					for (; j < 0x10; ++j)
						fputs(",$00", out_file);

					fputc('\n', out_file);
				}

				fclose(out_file);
			}

			MemoryStream_Destroy(&memory_stream);
		}
	}

	return exit_code;
}
