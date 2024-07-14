SRC = alfur.c elf.c
OBJ = ${SRC:.c=.o}

CC = tcc
CFLAGS = -Wall

all: alfur

.c.o:
	${CC} -c ${CFLAGS} $<

alfur: ${OBJ}
	${CC} -o $@ ${OBJ}

clean:
	rm -f alfur ${OBJ}
