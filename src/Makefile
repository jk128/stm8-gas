.PHONY: clean

OBJECTS=app.o as.o symbols.o messages.o read.o listing.o frags.o expr.o subsegs.o \
	write.o common.o sb.o depend.o macro.o hash.o stabs.o remap.o cond.o input-scrub.o \
	input-file.o output-file.o listing.o compress-debug.o ecoff.o ehopt.o dwarf2dbg.o \
	dw2gencfi.o atof-generic.o flonum-konst.o flonum-copy.o flonum-mult.o \
	config/tc-st8.o config/atof-ieee.o config/obj-elf.o
CFLAGS=-DHAVE_STDLIB_H -DHAVE_STRING_H -DSTM8 -DHAVE_ERRNO_H -I../include/
LDFLAGS=-lbfd -liberty -zmuldefs
BINARY=stm8-as

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

clean:
	rm $(OBJECTS) $(BINARY)
