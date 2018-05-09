// Clownacy's accurate Kosinski compressor.
// Creates identical output to Sega's own compressor.

// Note that Sega's compressor was riddled with errors,
// search 'Mistake' to find them.

#include "kosinski_compress.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#undef MIN
#undef MAX
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define MAX_MATCH_LENGTH 0xFD				// Mistake 1: This should be 0x100
#define MAX_MATCH_DISTANCE (0x2000 - MAX_MATCH_LENGTH)	// Mistake 2: This should just be 0x2000

#define TOTAL_DESCRIPTOR_BITS 16

static FILE *output_file;

static unsigned char *match_buffer;
static size_t match_buffer_index = 0;

static unsigned short descriptor;
static unsigned int descriptor_bits_remaining = TOTAL_DESCRIPTOR_BITS;

static void PutByte(unsigned char byte)
{
	static size_t match_buffer_size = 0;

	if (match_buffer_index + 1 > match_buffer_size)
		match_buffer = realloc(match_buffer, match_buffer_index + 1);

	match_buffer[match_buffer_index++] = byte;

}

static void PutDescriptorBit(bool bit)
{
	descriptor >>= 1;

	if (bit)
		descriptor |= 1 << (TOTAL_DESCRIPTOR_BITS - 1);

	if (--descriptor_bits_remaining == 0)
	{
		fwrite(&descriptor, 2, 1, output_file);
		fwrite(match_buffer, match_buffer_index, 1, output_file);

		descriptor_bits_remaining = TOTAL_DESCRIPTOR_BITS;
		match_buffer_index = 0;
	}
}

static void FlushData(void)
{
	descriptor >>= descriptor_bits_remaining;
	fwrite(&descriptor, 2, 1, output_file);
	fwrite(match_buffer, match_buffer_index, 1, output_file);
}

void KosinskiCompress(unsigned char *file_buffer, size_t file_size, FILE *p_output_file)
{
	output_file = p_output_file;

	unsigned char *file_pointer = file_buffer;

	while (file_pointer < file_buffer + file_size)
	{
		unsigned int max_match_distance = MIN(file_pointer - file_buffer, MAX_MATCH_DISTANCE);

		unsigned int longest_match_index;
		unsigned int longest_match_length = 0;
		for (unsigned int backsearch_index = 1; backsearch_index < max_match_distance + 1; ++backsearch_index)
		{
			unsigned int max_match_length = MIN(file_size - (file_pointer - file_buffer), MAX_MATCH_LENGTH);

			unsigned int match_length = 0;
			while (match_length < max_match_length && file_pointer[match_length] == file_pointer[-backsearch_index + match_length])
			{
				++match_length;
			}

			if (match_length > longest_match_length)
			{
				longest_match_index = backsearch_index;
				longest_match_length = match_length;
			}
		}

		if (longest_match_length >= 2 && longest_match_length <= 5 && longest_match_index < 256)	// Mistake 3: This should be '<= 256'
		{
			#ifndef SHUTUP
			printf("%X - Inline dictionary match found: %X, %X, %X\n", ftell(output_file) + match_buffer_index + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int match = longest_match_length - 2;

			PutDescriptorBit(false);
			PutDescriptorBit(false);
			PutDescriptorBit(match & 2);
			PutDescriptorBit(match & 1);
			PutByte(-longest_match_index);

			file_pointer += longest_match_length;
		}
		else if (longest_match_length >= 3 && longest_match_length < 10)
		{
			#ifndef SHUTUP
			printf("%X - Full match found: %X, %X, %X\n", ftell(output_file) + match_buffer_index + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int distance = -longest_match_index;
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutByte(distance & 0xFF);
			PutByte(((distance & 0xFF00) >> (8 - 3)) | (longest_match_length - 2));

			file_pointer += longest_match_length;
		}
		else if (longest_match_length >= 3)
		{
			#ifndef SHUTUP
			printf("%X - Extended full match found: %X, %X, %X\n", ftell(output_file) + match_buffer_index + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int distance = -longest_match_index;
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutByte(distance & 0xFF);
			PutByte((distance & 0xFF00) >> (8 - 3));
			PutByte(longest_match_length - 1);

			file_pointer += longest_match_length;
		}
		else
		{
			#ifndef SHUTUP
			printf("%X - Literal match found: %X at %X\n", ftell(output_file) + match_buffer_index + 2, *file_pointer, file_pointer - file_buffer);
			#endif
			PutDescriptorBit(true);
			PutByte(*file_pointer++);
		}
	}

	// Terminator match
	PutDescriptorBit(false);
	PutDescriptorBit(true);
	PutByte(0x00);
	PutByte(0xF0);
	PutByte(0x00);

	FlushData();

	// Pad to 0x10
	size_t bytes_remaining = (-ftell(output_file) & 0xF);
	for (unsigned int i = 0; i < bytes_remaining; ++i)
		fputc(0, output_file);
}
