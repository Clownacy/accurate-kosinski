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

/* Sega-accurate Kosinski compressor.
   Creates identical output to Sega's own compressor.

   Note that Sega's compressor was riddled with errors.
   Search 'Mistake' to find my reimplementations of them with explanations.

   Notably, Sega's compressor used a greedy compression algorithm.
   This doesn't give the best possible compression ratio, but it's fast, easy to
   implement, and can be done without loading the entire file into memory.
   The graph-theory-based 'perfect' compression algorithm used by clownlzss and
   mdcomp would not have been feasible on late-80s PCs. */

#include "kosinski-compress.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLIDING_WINDOW_SIZE 0x2000

#define MAX_MATCH_LENGTH 0xFD                                       /* Mistake 1: This should be 0x100 */
#define MAX_MATCH_DISTANCE (SLIDING_WINDOW_SIZE - MAX_MATCH_LENGTH) /* Mistake 2: This should just be SLIDING_WINDOW_SIZE */

#define TOTAL_DESCRIPTOR_BITS 16

/* The first 3 is for the 0xA000 boundary dummy match, and the other 3s are for the largest dictionary match size. */
#define MATCH_BUFFER_SIZE (3 + 3 * TOTAL_DESCRIPTOR_BITS)
static unsigned char match_buffer[MATCH_BUFFER_SIZE];
static size_t match_buffer_index;

static size_t output_position;

static unsigned int descriptor;
static unsigned int descriptor_bits_remaining;

/* Rather than load the entire file into memory, it appears that the original
   Kosinski compressor would stream data into a ring buffer, which matched the
   size of the LZSS sliding window (plus room for a little spill buffer).
   Okumura's 1989 LZSS compressor does this too, so it appears that this was a
   common technique back then. */
static unsigned char ring_buffer[SLIDING_WINDOW_SIZE + MAX_MATCH_LENGTH - 1];

static void FlushData(const KosinskiCompressCallbacks* const callbacks)
{
	size_t i;

	descriptor >>= descriptor_bits_remaining;

	/* Descriptors are stored byte-swapped, so it's possible that the original
	   compressor was designed for a little-endian CPU and that it did this:
	   fwrite(&descriptor, 2, 1, output_file); */
	for (i = 0; i < TOTAL_DESCRIPTOR_BITS / 8; ++i)
		callbacks->write_byte((void*)callbacks->write_byte_user_data, (descriptor >> (i * 8)) & 0xFF);

	for (i = 0; i < match_buffer_index; ++i)
		callbacks->write_byte((void*)callbacks->write_byte_user_data, match_buffer[i]);

	output_position += TOTAL_DESCRIPTOR_BITS / 8 + match_buffer_index;
}

static void PutMatchByte(const unsigned char byte)
{
	match_buffer[match_buffer_index++] = byte;
}

static void PutDescriptorBit(const cc_bool bit, const KosinskiCompressCallbacks* const callbacks)
{
	descriptor >>= 1;

	if (bit)
		descriptor |= 1u << (TOTAL_DESCRIPTOR_BITS - 1);

	if (--descriptor_bits_remaining == 0)
	{
		FlushData(callbacks);

		descriptor_bits_remaining = TOTAL_DESCRIPTOR_BITS;
		match_buffer_index = 0; /* TODO: Move this to `FlushData`. */
	}
}

static unsigned long GetOutputPosition(void)
{
	return (unsigned long)(output_position + TOTAL_DESCRIPTOR_BITS / 8 + match_buffer_index);
}

void KosinskiCompress(const KosinskiCompressCallbacks* const callbacks, const cc_bool print_debug_messages)
{
	size_t read_index = 0;
	size_t file_index = 0;
	size_t dummy_counter = 0;

	output_position = 0;
	match_buffer_index = 0;
	descriptor_bits_remaining = TOTAL_DESCRIPTOR_BITS;

	/* Initialise the ring buffer with data from the file */
	for (; read_index < MAX_MATCH_LENGTH; ++read_index)
	{
		const unsigned int byte = callbacks->read_byte((void*)callbacks->read_byte_user_data);

		if (byte == (unsigned int)-1)
			break;

		ring_buffer[read_index] = byte;
	}

	/* Fill the remainder of the ring buffer with zero. We know that
	   the original Kosinski compressor did this because of Mistake 6. */
	memset(&ring_buffer[read_index], 0, sizeof(ring_buffer) - read_index);

	while (file_index != read_index)
	{
		size_t i;

		const size_t max_match_distance = CC_MIN(file_index, MAX_MATCH_DISTANCE);

		/* Search backwards for previous occurances of the current data */
		size_t longest_match_index = 0;
		size_t longest_match_length = 0;
		size_t backsearch_index;
		for (backsearch_index = 1; backsearch_index < max_match_distance + 1; ++backsearch_index)
		{
			/* Mistake 6: `match_length` always counts up to `MAX_MATCH_LENGTH`, even if it means reading
			   past the end of the file. Because the ring buffer isn't updated once the end of the file is
			   reached, this results in leftover values from earlier in the file being read instead.
			   This bug causes the final match in the file to sometimes ignore suitable nearby data in
			   favour of data earlier in the file, even if it means using a larger match type. This is
			   because the chosen data just so happened to be followed by the same pattern of bytes that
			   the buggy search read from the ring buffer, while the nearby data did not. */
			size_t match_length = 0;
			const unsigned char *current_data = &ring_buffer[file_index % SLIDING_WINDOW_SIZE];
			const unsigned char *previous_data = &ring_buffer[(file_index - backsearch_index) % SLIDING_WINDOW_SIZE];
			while (match_length < MAX_MATCH_LENGTH && *current_data++ == *previous_data++)
				++match_length;

			if (match_length > longest_match_length)
			{
				longest_match_index = backsearch_index;
				longest_match_length = match_length;
			}
		}

		/* If the match is longer than the remainder of the file, reduce it to the proper size. See Mistake 6 for more info. */
		longest_match_length = CC_MIN(longest_match_length, read_index - file_index);

		/* Mistake 5: This is completely pointless.
		   For some reason, the original compressor would insert a dummy match
		   before the first match that starts after 0xA000.
		   Perhaps this was intended for detecting corrupted data? Maybe the PC
		   Kosinski decompressor would expect this type of match to appear every
		   0xA000 bytes, and if it didn't, then it would bail and print an error
		   message to the user telling them that the compressed data is corrupt. */
		if (dummy_counter >= 0xA000)
		{
			dummy_counter %= 0xA000;

			if (print_debug_messages)
				fprintf(stderr, "%lX - 0xA000 boundary flag: %lX\n", GetOutputPosition(), (unsigned long)file_index);

			/* 0xA000 boundary match */
			PutDescriptorBit(cc_false, callbacks);
			PutDescriptorBit(cc_true, callbacks);
			PutMatchByte(0x00);
			PutMatchByte(0xF0);	/* Honestly, I have no idea why this isn't just 0. I guess it's so you can spot it in a hex editor? */
			PutMatchByte(0x01);
		}

		/* Select the optimal encoding for the current match */
		if (longest_match_length >= 2 && longest_match_length <= 5 && longest_match_index < 0x100) /* Mistake 3: This should be '<= 0x100' */
		{
			/* Short distance, shortest length */
			const size_t length = longest_match_length - 2;

			if (print_debug_messages)
				fprintf(stderr, "%lX - Inline dictionary match found: %lX, %lX, %lX\n", GetOutputPosition(), (unsigned long)file_index, (unsigned long)(file_index - longest_match_index), (unsigned long)longest_match_length);

			PutDescriptorBit(cc_false, callbacks);
			PutDescriptorBit(cc_false, callbacks);
			PutDescriptorBit((length & 2) != 0, callbacks);
			PutDescriptorBit((length & 1) != 0, callbacks);
			PutMatchByte(-longest_match_index & 0xFF);
		}
		else if (longest_match_length >= 3 && longest_match_length <= 9)
		{
			/* Long distance, short length */
			const size_t distance = -longest_match_index;

			if (print_debug_messages)
				fprintf(stderr, "%lX - Full match found: %lX, %lX, %lX\n", GetOutputPosition(), (unsigned long)file_index, (unsigned long)(file_index - longest_match_index), (unsigned long)longest_match_length);

			PutDescriptorBit(cc_false, callbacks);
			PutDescriptorBit(cc_true, callbacks);
			PutMatchByte(distance & 0xFF);
			PutMatchByte(((distance >> (8 - 3)) & 0xF8) | ((longest_match_length - 2) & 7));
		}
		else if (longest_match_length >= 3)
		{
			/* Long distance, long length */
			const size_t distance = -longest_match_index;

			if (print_debug_messages)
				fprintf(stderr, "%lX - Extended full match found: %lX, %lX, %lX\n", GetOutputPosition(), (unsigned long)file_index, (unsigned long)(file_index - longest_match_index), (unsigned long)longest_match_length);

			PutDescriptorBit(cc_false, callbacks);
			PutDescriptorBit(cc_true, callbacks);
			PutMatchByte(distance & 0xFF);
			PutMatchByte((distance >> (8 - 3)) & 0xF8);
			PutMatchByte(longest_match_length - 1);
		}
		else
		{
			if (print_debug_messages)
				fprintf(stderr, "%lX - Literal match found: %X at %lX\n", GetOutputPosition(), ring_buffer[file_index % SLIDING_WINDOW_SIZE], (unsigned long)file_index);

			/* Match was too small to encode; do a literal match instead */
			longest_match_length = 1;

			PutDescriptorBit(cc_true, callbacks);
			PutMatchByte(ring_buffer[file_index % SLIDING_WINDOW_SIZE]);
		}

		/* Update the ring buffer with bytes from the file */
		for (i = 0; i < longest_match_length; ++i)
		{
			const unsigned int byte = callbacks->read_byte((void*)callbacks->read_byte_user_data);

			if (byte == (unsigned int)-1)
			{
				break;
			}
			else
			{
				const size_t ring_buffer_index = read_index++ % SLIDING_WINDOW_SIZE;

				ring_buffer[ring_buffer_index] = byte;

				/* Read into a little spill buffer, so that string comparisons
				   don't have to wrap back around to the start of the ring buffer */
				if (ring_buffer_index < MAX_MATCH_LENGTH - 1)
					ring_buffer[SLIDING_WINDOW_SIZE + ring_buffer_index] = byte;
			}
		}

		file_index += longest_match_length;
		dummy_counter += longest_match_length;
	}

	if (print_debug_messages)
		fprintf(stderr, "%lX - Terminator: %lX\n", GetOutputPosition(), (unsigned long)file_index);

	/* Terminator match */
	PutDescriptorBit(cc_false, callbacks);
	PutDescriptorBit(cc_true, callbacks);
	PutMatchByte(0x00);
	PutMatchByte(0xF0);	/* Honestly, I have no idea why this isn't just 0. I guess it's so you can spot it in a hex editor? */
	PutMatchByte(0x00);

	FlushData(callbacks);

	/* Mistake 4: There's absolutely no reason to do this.
	   This might have been because the original compressor's ASM output could
	   only write exactly 0x10 values per dc.b instruction. */

	/* Pad to 0x10 bytes */
	while (output_position++ % 0x10 != 0)
		callbacks->write_byte((void*)callbacks->write_byte_user_data, 0);
}
