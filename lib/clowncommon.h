/*
Copyright (c) 2021 Clownacy

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

#ifndef CLOWNCOMMON_H
#define CLOWNCOMMON_H

/* Boolean */
typedef unsigned char cc_bool_small;
typedef unsigned int cc_bool_fast;
enum
{
	CC_FALSE = 0,
	CC_TRUE = 1
};

/* Common macros */
#define CC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CC_CLAMP(x, min, max) (CC_MIN((max), CC_MAX((min), (x))))
#define CC_COUNT_OF(array) (sizeof(array) / sizeof(*array))

#endif /* CLOWNCOMMON_H */
