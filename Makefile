#Makefile with basic concepts

# Define compiler and options
CC=g++
CFLAGS= `pkg-config --cflags opencv gtk+-2.0`
LDFLAGS+=`pkg-config --libs opencv gtk+-2.0`


.PHONY: all clean
# avoid calling make without option
# ONLY FOR THIS EXERCICE
all: projetX

projetX: lib_gtk.o struct.o traitement.o projetX.o 
	$(CC) -o projetX lib_gtk.o struct.o traitement.o projetX.o  $(LDFLAGS) -lm 

struct.o: struct.h struct.c
	gcc -std=c99 -c struct.c -o struct.o

projetX.o: projetX.c 
	$(CC) $(CFLAGS) -c projetX.c 

traitement.o: traitement.h traitement.c
	$(CC) $(CFLAGS) -c traitement.c `pkg-config opencv --cflags` 
lib_gtk.o:
	$(CC) -c lib_gtk.c $(CFLAGS) $<

clean::
	rm -f *~ *.o
	rm -f projetX

re: clean all
# END (do not delete)


