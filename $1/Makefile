OBJ=${:%.c=%.o}

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -I./
DBGFLAGS=-g3 -ggdb3

all: dollar1

dollar1: ${OBJ} struct.c dollar1.c
	$(CC) $(CFLAGS) ${DBGFLAGS} dollar1.c -o dollar1 ${OBJ} -lm

clean:
	@${RM} -v *.o
	@${RM} -v dollar1
