CFLAGS=-I./mtx/lzcomp
all: eotparser

eotparser: liblzcomp.a libeot.a libmtx.a eotparser.o
	$(CC) -o eotparser eotparser.o mtx/libmtx.a eot/libeot.a mtx/lzcomp/liblzcomp.a $(CFLAGS)

eotparser.o:
	$(CC) -c eotparser.c

liblzcomp.a: mtx/lzcomp/ahuff.o mtx/lzcomp/bitio.o mtx/lzcomp/lzcomp.o mtx/lzcomp/mtxmem.o
	rm -f mtx/lzcomp/liblzcomp.a
	ar q mtx/lzcomp/liblzcomp.a mtx/lzcomp/ahuff.o mtx/lzcomp/bitio.o mtx/lzcomp/lzcomp.o mtx/lzcomp/mtxmem.o

libeot.a: eot/libeot.o eot/properties.o
	rm -f eot/libeot.a
	ar q eot/libeot.a eot/libeot.o eot/properties.o

libmtx.a: mtx/mtx.o
	rm -f mtx/libmtx.a
	ar q mtx/libmtx.a mtx/mtx.o



clean:
	rm -Rf *.o *.a eotparser core *.core
