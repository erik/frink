CC=clang
INCS= -Iatto/src/
LIBS= atto/libatto.a
CFLAGS= -Wall -Wextra -std=c99 $(INCS)
LNFLAGS= $(LIBS)
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
EXE=frink

all: atto $(EXE)

$(EXE): $(OBJ)
	@echo "link $(EXE)"
	@$(CC) $(OBJ) $(CFLAGS) $(LNFLAGS) -o$(EXE)

src/compiler.o: src/compiler.c src/compiler.h
src/loader.o: src/loader.c src/loader.h src/frink.h
src/main.o: src/main.c

.c.o:
	@echo "   cc $<"
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXE) $(OBJ)

distclean: clean
	rm -rf atto/

todo:
	@find src -type f | xargs grep -n -i "TODO"
	@find src -type f | xargs grep -n -i "FIXME"

loc: 
	@wc -l src/*.[ch]

# requires sloccount
sloc:
	@sloccount src lib | grep '(SLOC)'

atto:
	@echo "cloning atto"
	@git clone -q git://github.com/boredomist/atto.git
	@echo "building atto"
	@cd atto && make lib

.PHONY= loc sloc todo atto
# DO NOT DELETE

src/compiler.o: src/compiler.h atto/src/block.h atto/src/atto.h
src/compiler.o: atto/src/config.h /usr/include/stdlib.h
src/compiler.o: /usr/include/features.h /usr/include/bits/predefs.h
src/compiler.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/compiler.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
src/compiler.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
src/compiler.o: /usr/include/endian.h /usr/include/bits/endian.h
src/compiler.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
src/compiler.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/compiler.o: /usr/include/time.h /usr/include/sys/select.h
src/compiler.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
src/compiler.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
src/compiler.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
src/compiler.o: /usr/include/stdio.h /usr/include/libio.h
src/compiler.o: /usr/include/_G_config.h /usr/include/wchar.h
src/compiler.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
src/compiler.o: /usr/include/stdint.h /usr/include/bits/wchar.h
src/compiler.o: atto/src/vec.h atto/src/value.h atto/src/stack.h
src/compiler.o: atto/src/opcodes.h
src/loader.o: src/loader.h src/frink.h
src/main.o: /usr/include/stdio.h /usr/include/features.h
src/main.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
src/main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/main.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
src/main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
src/main.o: /usr/include/_G_config.h /usr/include/wchar.h
src/main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
src/main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
src/main.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
src/main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
src/main.o: /usr/include/sys/types.h /usr/include/time.h
src/main.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/main.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
src/main.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
src/main.o: /usr/include/alloca.h
