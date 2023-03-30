This is a library for compressing and decompressing files in Sega's "Kosinski"
format (an LZSS variant). The goal is to produce a compressor that outputs data
identical to Sega's own compressor. Moduled Kosinski is also supported.

https://segaretro.org/Kosinski_compression

Included are basic frontends for a compressor, a decompressor, and a comparison
utility.

Sega's compressor had a fair few bugs, causing it to produce files with
a sub-optimal compression ratio. Lucky, none of these issues harm the integrity
of the compressed data. See `lib/kosinski-compress.c` for more info.
