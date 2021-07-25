CFLAGS := -O2 -std=c99 -DNDEBUG -Wall -Wextra -pedantic

all: compare kosinski_compress kosinski_compress_asm kosinski_decompress kosinski_compress_d kosinski_decompress_d kosinskim_compress_d kosinskim_decompress_d kosinskim_compare

compare: main_compare.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress: main_compress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_asm: main_compress_asm.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_decompress: main_decompress.c lib/kosinski_decompress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_d: main_compress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinski_decompress_d: main_decompress.c lib/kosinski_decompress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_compress_d: main_moduled_compress.c lib/kosinski_moduled_compress.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_decompress_d: main_moduled_decompress.c lib/kosinski_moduled_decompress.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_compare: main_moduled_compare.c lib/kosinski_moduled_compress.c lib/kosinski_decompress.c lib/kosinski_moduled_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
