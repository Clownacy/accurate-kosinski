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
#include <string.h>

#include "memory_stream.h"
#include "minmax.h"

#define MAX_MATCH_LENGTH 0xFD				// Mistake 1: This should be 0x100
#define MAX_MATCH_DISTANCE (0x2000 - MAX_MATCH_LENGTH)	// Mistake 2: This should just be 0x2000

#define TOTAL_DESCRIPTOR_BITS 16

static MemoryStream *output_stream;
static MemoryStream *match_stream;

static unsigned short descriptor;
static unsigned int descriptor_bits_remaining;

static void FlushData(void)
{
	descriptor >>= descriptor_bits_remaining;

	// Descriptors are stored byte-swapped, so it's possible the
	// original compressor did this:
	//fwrite(&descriptor, 2, 1, output_file);
	// For portability, however, we're doing it manually
	MemoryStream_WriteByte(output_stream, descriptor & 0xFF);
	MemoryStream_WriteByte(output_stream, descriptor >> 8);

	size_t match_buffer_size = MemoryStream_GetIndex(match_stream);
	unsigned char *match_buffer = MemoryStream_GetBuffer(match_stream);

	MemoryStream_WriteBytes(output_stream, match_buffer, match_buffer_size);
}

static void PutMatchByte(unsigned char byte)
{
	MemoryStream_WriteByte(match_stream, byte);
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
		MemoryStream_Reset(match_stream);
	}
}

size_t KosinskiCompress(unsigned char *file_buffer, size_t file_size, unsigned char **p_output_buffer)
{
	output_stream = MemoryStream_Init(0x100);
	match_stream = MemoryStream_Init(0x10);

	descriptor_bits_remaining = TOTAL_DESCRIPTOR_BITS;

	unsigned char *file_pointer = file_buffer;
	unsigned int last_src_file_index = 0;

	while (file_pointer < file_buffer + file_size)
	{
		// Mistake 5: This is completely pointless
		// For some reason, the original compressor would insert a dummy match
		// before the first match that copies to after 0xA000
		if (file_pointer - file_buffer >= 0xA000 && last_src_file_index < 0xA000)
		{
			#ifdef DEBUG
			printf("%X - Dummy terminator: %X\n", MemoryStream_GetIndex(output_stream) + MemoryStream_GetIndex(match_stream) + 2, file_pointer - file_buffer);
			#endif

			// Terminator match
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutMatchByte(0x00);
			PutMatchByte(0xF0);	// Honestly, I have no idea why this isn't just 0. I guess it's so you can spot it in a hex editor?
			PutMatchByte(0x01);
		}

		last_src_file_index = file_pointer - file_buffer;

		const unsigned int max_match_distance = MIN(file_pointer - file_buffer, MAX_MATCH_DISTANCE);
		const unsigned int max_match_length = MIN(file_size - (file_pointer - file_buffer), MAX_MATCH_LENGTH);

		unsigned int longest_match_index;
		unsigned int longest_match_length = 0;
		for (unsigned int backsearch_index = 1; backsearch_index < max_match_distance + 1; ++backsearch_index)
		{

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
			printf("%X - Inline dictionary match found: %X, %X, %X\n", MemoryStream_GetIndex(output_stream) + MemoryStream_GetIndex(match_stream) + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int length = longest_match_length - 2;

			PutDescriptorBit(false);
			PutDescriptorBit(false);
			PutDescriptorBit(length & 2);
			PutDescriptorBit(length & 1);
			PutMatchByte(-longest_match_index);

			file_pointer += longest_match_length;
		}
		else if (longest_match_length >= 3 && longest_match_length < 10)
		{
			#ifdef DEBUG
			printf("%X - Full match found: %X, %X, %X\n", MemoryStream_GetIndex(output_stream) + MemoryStream_GetIndex(match_stream) + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int distance = -longest_match_index;
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutMatchByte(distance & 0xFF);
			PutMatchByte(((distance >> (8 - 3)) & 0xF8) | ((longest_match_length - 2) & 7));

			file_pointer += longest_match_length;
		}
		else if (longest_match_length >= 3)
		{
			#ifdef DEBUG
			printf("%X - Extended full match found: %X, %X, %X\n", MemoryStream_GetIndex(output_stream) + MemoryStream_GetIndex(match_stream) + 2, file_pointer - file_buffer, file_pointer - file_buffer - longest_match_index, longest_match_length);
			#endif

			const unsigned int distance = -longest_match_index;
			PutDescriptorBit(false);
			PutDescriptorBit(true);
			PutMatchByte(distance & 0xFF);
			PutMatchByte((distance >> (8 - 3)) & 0xF8);
			PutMatchByte(longest_match_length - 1);

			file_pointer += longest_match_length;
		}
		else
		{
			#ifdef DEBUG
			printf("%X - Literal match found: %X at %X\n", MemoryStream_GetIndex(output_stream) + MemoryStream_GetIndex(match_stream) + 2, *file_pointer, file_pointer - file_buffer);
			#endif

			PutDescriptorBit(true);
			PutMatchByte(*file_pointer++);
		}
	}

	#ifdef DEBUG
	printf("%X - Terminator: %X\n", MemoryStream_GetIndex(output_stream) + MemoryStream_GetIndex(match_stream) + 2, file_pointer - file_buffer);
	#endif

	// Terminator match
	PutDescriptorBit(false);
	PutDescriptorBit(true);
	PutMatchByte(0x00);
	PutMatchByte(0xF0);	// Honestly, I have no idea why this isn't just 0. I guess it's so you can spot it in a hex editor?
	PutMatchByte(0x00);

	FlushData();

	// Destory match_buffer
	free(MemoryStream_GetBuffer(match_stream));
	free(match_stream);

	// Mistake 4: There's absolutely no reason to do this (except for Moduled Kosinski)
	// Pad to 0x10
	size_t bytes_remaining = -MemoryStream_GetIndex(output_stream) & 0xF;
	for (unsigned int i = 0; i < bytes_remaining; ++i)
		MemoryStream_WriteByte(output_stream, 0);

	size_t output_buffer_size = MemoryStream_GetIndex(output_stream);
	unsigned char *output_buffer = MemoryStream_GetBuffer(output_stream);

	free(output_stream);

	*p_output_buffer = output_buffer;
	return output_buffer_size;
}
