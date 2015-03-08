#Makefile with basic concepts

# Define compiler and options
CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O2 

# avoid calling make without option
# ONLY FOR THIS EXERCICE
all: projetX

projetX: traitement.o projetX.o
	$(CC) $(CFLAGS) traitement.o projetX.o -o projetX `pkg-config opencv --libs` -lm     

projetX.o: projetX.c
	$(CC) $(CFLAGS) -c projetX.c 

traitement.o: traitement.h traitement.c
	$(CC) $(CFLAGS) -c traitement.c `pkg-config opencv --cflags` 

clean::
	rm -f *~ *.o
	rm -f projetX
# END (do not delete)
