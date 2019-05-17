CFLAGS := -O2 -s -std=c99 -fno-ident -Wall -Wextra -pedantic -Wno-maybe-uninitialized

all: compare.exe kosinski_compress.exe kosinski_compress_asm.exe kosinski_decompress.exe kosinski_compress_d.exe kosinski_decompress_d.exe kosinskim_compress_d.exe kosinskim_decompress_d.exe kosinskim_compare.exe

compare.exe: main_compare.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress.exe: main_compress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_asm.exe: main_compress_asm.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_decompress.exe: main_decompress.c lib/kosinski_decompress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_d.exe: main_compress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinski_decompress_d.exe: main_decompress.c lib/kosinski_decompress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_compress_d.exe: main_moduled_compress.c lib/kosinski_moduled_compress.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_decompress_d.exe: main_moduled_decompress.c lib/kosinski_moduled_decompress.c lib/kosinski_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_compare.exe: main_moduled_compare.c lib/kosinski_moduled_compress.c lib/kosinski_decompress.c lib/kosinski_moduled_decompress.c lib/kosinski_compress.c load_file_to_buffer.c lib/memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
