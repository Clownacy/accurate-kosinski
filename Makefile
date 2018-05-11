CFLAGS := -O3 -s -static -std=c11 -fomit-frame-pointer -fno-ident -Wall -Wextra

all: kos.exe decomp.exe compare.exe kosinski_compress.exe

kos.exe: main.c kosinski_compress.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

decomp.exe: main2.c kosinski_decompress.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

compare.exe: main3.c kosinski_decompress.c kosinski_compress.c
	$(CC) $(CFLAGS) -DSHUTUP -o $@ $^ $(LIBS)

kosinski_compress.exe: main.c kosinski_compress.c
	$(CC) $(CFLAGS) -DSHUTUP -o $@ $^ $(LIBS)
