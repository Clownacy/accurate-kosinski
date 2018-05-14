// Copyright (c) 2018 Clownacy

// Sega-accurate Kosinski compressor.
// Creates identical output to Sega's own compressor.

// Note that Sega's compressor was riddled with errors.
// Search 'Mistake' to find my reimplementations of them.

// Notably, Sega's compressor used the 'longest-match' algorithm.
// This doesn't give the best possible compression ratio, but it's fast,
// easy to implement, and can be done without loading the entire file into
// memory. The graph-theory-based algorithm Flamewing used might not have
// been feasible on late-80s PCs.

// Unfortunately, there's one bug I can't emulate: it seems the original
// compressor would accidentally read past the end of the uncompressed
// file, leading to the final match reading from an odd place in the
// dictionary. This would be because, instead of searching for 0FAE66,
// it would search for 0FAE6614 instead. Usually, the match has a 0 added
// to the end of it, but other times it's a different number entirely.

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

static unsigned char *output_buffer;
static size_t output_buffer_index;

static unsigned short descriptor;
static unsigned int descriptor_bits_remaining;

static void PutByte(unsigned char byte)
{
	// Since descriptors have to come before the data they represent,
	// we have to buffer the data here, and only flush it to disk when
	// the descriptor is output.
	static size_t output_buffer_size = 0;

	if (output_buffer_index + 1 > output_buffer_size)
	{
		output_buffer_size += 0x10;
		output_buffer = realloc(output_buffer, output_buffer_size);
	}

	output_buffer[output_buffer_index++] = byte;
}

static void FlushData(void)
{
	descriptor >>= descriptor_bits_remaining;

	// Descriptors are stored byte-swapped, so it's possible the
	// original compressor did this:
	//fwrite(&descriptor, 2, 1, output_file);
	// For portability, however, we're doing it manually
	fputc(descriptor & 0xFF, output_file);
	fputc(descriptor >> 8, output_file);

	fwrite(output_buffer, output_buffer_index, 1, output_file);
}

static void PutDescriptorBit(bool bit)
{
	descriptor >>= 1;

	if (bit)
		descriptor |= 1 << (TOTAL_DESCRIPTOR_BITS - 1);

	if (--descriptor_bits_remaining == 0)
	{
		FlushData();

		descriptor_bits_remaining = TOTAL_DESCRIPTOR_BITS;
		output_buffer_index = 0;
	}
}

void KosinskiCompress(unsigned char *file_buffer, size_t file_size, FILE *p_output_file)
{
	output_file = p_output_file;
	output_buffer_index = 0;
	descriptor_bits_remaining = TOTAL_DESCRIPTOR_BITS;

	unsigned char *file_pointer = file_buffer;
	unsigned int last_src_file_index = 0;

	while (file_pointer < file_buffer + file_size)
	{
		// Mistake 5: This is completely pointless
		// For some reason, the original compressor would insert dummy matches
		// before the first match that copies to after 0xA000
		if (file_pointer - file_buffer >= 0xA000 && last_src_file_index < 0xA000)
		{
			#ifdef DEBUG
			printf("%lX - Dummy terminator: %X\n", ftell(output_file) + output_buffer_index + 2, file_pointer - file_buffer);
			#endif

			// Terminator match
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutByte(0x00);
			PutByte(0xF0);	// Honestly, I have no idea why this isn't just 0. I guess it's so you can spot it in a hex editor?
			PutByte(0x01);
		}

		last_src_file_index = file_pointer - file_buffer;

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
			#ifdef DEBUG
			printf("%lX - Inline dictionary match found: %X, %X, %X\n", ftell(output_file) + output_buffer_index + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int length = longest_match_length - 2;

			PutDescriptorBit(false);
			PutDescriptorBit(false);
			PutDescriptorBit(length & 2);
			PutDescriptorBit(length & 1);
			PutByte(-longest_match_index);

			file_pointer += longest_match_length;
		}
		else if (longest_match_length >= 3 && longest_match_length < 10)
		{
			#ifdef DEBUG
			printf("%lX - Full match found: %X, %X, %X\n", ftell(output_file) + output_buffer_index + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int distance = -longest_match_index;
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutByte(distance & 0xFF);
			PutByte(((distance >> (8 - 3)) & 0xF8) | ((longest_match_length - 2) & 7));

			file_pointer += longest_match_length;
		}
		else if (longest_match_length >= 3)
		{
			#ifdef DEBUG
			printf("%lX - Extended full match found: %X, %X, %X\n", ftell(output_file) + output_buffer_index + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int distance = -longest_match_index;
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutByte(distance & 0xFF);
			PutByte((distance >> (8 - 3)) & 0xF8);
			PutByte(longest_match_length - 1);

			file_pointer += longest_match_length;
		}
		else
		{
			#ifdef DEBUG
			printf("%lX - Literal match found: %X at %X\n", ftell(output_file) + output_buffer_index + 2, *file_pointer, file_pointer - file_buffer);
			#endif

			PutDescriptorBit(true);
			PutByte(*file_pointer++);
		}
	}

	#ifdef DEBUG
	printf("%lX - Terminator: %X\n", ftell(output_file) + output_buffer_index + 2, file_pointer - file_buffer);
	#endif

	// Terminator match
	PutDescriptorBit(false);
	PutDescriptorBit(true);
	PutByte(0x00);
	PutByte(0xF0);	// Honestly, I have no idea why this isn't just 0. I guess it's so you can spot it in a hex editor?
	PutByte(0x00);

	FlushData();

	// Mistake 4: There's absolutely no reason to do this
	// Pad to 0x10
	size_t bytes_remaining = -ftell(output_file) & 0xF;
	for (unsigned int i = 0; i < bytes_remaining; ++i)
		fputc(0, output_file);
}
