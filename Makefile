CC ?= gcc
# CFLAGS ?= -std=c99
CFDEBUG = -pedantic -Wall -g

EXEC = cobject
SRCS = cobject.c main.c
OBJS = ${SRCS:.c=.o}

all: ${EXEC}

.c.o:
	${CC} ${CFLAGS} -o $@ -c $<

${EXEC}: ${OBJS}
	${CC} ${LDFLAGS} -o ${EXEC} ${OBJS}

lib: ${OBJS}
	${CC} -c cobject.c -o cobject.o
	ar rcs libcobject.a cobject.o

debug: ${EXEC}
debug: CC += ${CFDEBUG}

test: debug
	./${EXEC}

clean:
	rm -rf ./*.o
	rm -rf ./*.a
	rm -rf ./${EXEC}

.PHONY: all debug clean
