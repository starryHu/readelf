all:myreadelf

INC := include/

CFLAGS := -Wall -I${INC}
SRC := readelf.c \
	elf_func.c

myreadelf:${SRC}
	@gcc ${CFLAGS} $^ -o $@
