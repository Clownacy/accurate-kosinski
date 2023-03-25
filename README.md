# Accurate Kosinski
This is a library for compressing and decompressing files in Sega's "Kosinski"
format (an LZSS variant). The goal is to produce a compressor that outputs data
identical to Sega's own compressor. Moduled Kosinski is also supported.

Included are basic frontends for a compressor, a decompressor, and a comparison
utility.

Sega's compressor had a fair few bugs, causing it to produce files with
sub-optimal compression ratios. Lucky, none of these issues harm the integrity
of the compressed data. See `lib/kosinski_compress.c` for more info.

# Building
```bash
mkdir build
cd build
cmake .. <OPTIONS>
cmake --build . --config Release
```
## Available options
### Build kcompare and kmcompare for tests
```-DCOMPARE="ON"```

### Enable Debug information
```-DDEBUG="ON"```

### Only build library
```-DONLYLIBRARY="ON"```