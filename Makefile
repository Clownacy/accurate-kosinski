CFLAGS := -O3 -s -static -std=c11 -fomit-frame-pointer -fno-ident -Wall -Wextra -Wno-maybe-uninitialized

all: compare.exe kosinski_compress.exe kosinski_decompress.exe kosinski_compress_d.exe kosinski_decompress_d.exe kosinskim_compress_d.exe kosinskim_decompress_d.exe kosinskim_compare.exe

compare.exe: main_compare.c kosinski_decompress.c kosinski_compress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress.exe: main_compress.c kosinski_compress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_decompress.exe: main_decompress.c kosinski_decompress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_d.exe: main_compress.c kosinski_compress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinski_decompress_d.exe: main_decompress.c kosinski_decompress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_compress_d.exe: main_moduled_compress.c kosinski_moduled_compress.c kosinski_decompress.c kosinski_compress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_decompress_d.exe: main_moduled_decompress.c kosinski_moduled_decompress.c kosinski_decompress.c kosinski_compress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinskim_compare.exe: main_moduled_compare.c kosinski_moduled_compress.c kosinski_decompress.c kosinski_moduled_decompress.c  kosinski_compress.c load_file_to_buffer.c memory_stream.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
