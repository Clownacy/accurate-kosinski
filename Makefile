CFLAGS := -O3 -s -static -std=c11 -fomit-frame-pointer -fno-ident -Wall -Wextra

all: compare.exe kosinski_compress.exe kosinski_decompress.exe kosinski_compress_d.exe kosinski_decompress_d.exe

compare.exe: main3.c kosinski_decompress.c kosinski_compress.c load_file_to_buffer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress.exe: main.c kosinski_compress.c load_file_to_buffer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_decompress.exe: main2.c kosinski_decompress.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

kosinski_compress_d.exe: main.c kosinski_compress.c load_file_to_buffer.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)

kosinski_decompress_d.exe: main2.c kosinski_decompress.c
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LIBS)
