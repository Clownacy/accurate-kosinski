CFLAGS := -O3 -s -static -std=c11 -fomit-frame-pointer -fno-ident

all: kos.exe decomp.exe

kos.exe: main.c kosinski.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

decomp.exe: decomp.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
