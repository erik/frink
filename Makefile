CSRC := $(shell find src -name "*.c")
CHDR := $(shell find include -name "*.c")

COBJ := $(CSRC:.c=.o)

CC := clang

LIBS := atto/libatto.a

CFLAGS  := -Wall -Wextra -std=c99 -Iatto/src/ -Iatto/include -Iinclude/
LNFLAGS := $(LIBS) -lm

EXE :=frink

all: atto $(COBJ) $(EXE)

$(EXE): $(COBJ)
	@echo "link $(EXE)"
	@$(CC) $(COBJ) $(LNFLAGS) -o$(EXE)

%.o: %.c
	@echo "   cc $<"
	@$(CC) -c $(CFLAGS) $< -o $@

debug:
	@cd atto && make clean debug lib
	@$(MAKE) "CFLAGS=$(CFLAGS) -g -O0"

clean:
	rm -f $(EXE) $(COBJ)

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
	@cd atto && make all lib

.PHONY= loc sloc todo atto all clean distclean debug
