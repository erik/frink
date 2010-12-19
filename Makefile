CC=clang
INCS= -Iatto/src/
LIBS= atto/libatto.a
CFLAGS= -Wall -Wextra -std=c99 $(INCS) -g
LNFLAGS= $(LIBS) -lm
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
