CFLAGS := -std=c99 -Wall -Wextra -pedantic -Wshift-overflow=2

ifeq ($(DEBUG), 1)
  CFLAGS := -Og -ggdb3 -fsanitize=address -fsanitize=undefined -fwrapv
else
  CFLAGS := -O2 -DNDEBUG
endif

all: kosinski_compress kosinski_compress_asm kosinski_decompress kosinski_compress_d kosinski_decompress_d kosinski_compare kosinskim_compress_d kosinskim_decompress_d kosinskim_compare

kosinski_compress: main_compress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_asm: main_compress_asm.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

kosinski_decompress: main_decompress.c lib/kosinski_decompress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_d: main_compress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinski_decompress_d: main_decompress.c lib/kosinski_decompress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinski_compare: main_compare.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

kosinskim_compress_d: main_moduled_compress.c lib/kosinski_moduled_compress.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_decompress_d: main_moduled_decompress.c lib/kosinski_moduled_decompress.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_compare: main_moduled_compare.c lib/kosinski_moduled_compress.c lib/kosinski_decompress.c lib/kosinski_moduled_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
