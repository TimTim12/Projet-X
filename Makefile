#Makefile with basic concepts

# Define compiler and options
CC=g++
CFLAGS= `pkg-config --cflags opencv gtk+-2.0`
LDFLAGS+=`pkg-config --libs opencv gtk+-2.0`

.PHONY: all clean
# avoid calling make without option
# ONLY FOR THIS EXERCICE
all: projetX

projetX: lib_gtk.o traitement_gtk.o projetX.o
	$(CC) -o projetX lib_gtk.o traitement_gtk.o projetX.o $(LDFLAGS) -lm 

projetX.o: projetX.c
	$(CC) $(CFLAGS) -c projetX.c 

traitement_gtk.o: traitement_gtk.h traitement_gtk.c
	$(CC) $(CFLAGS) -c traitement_gtk.c `pkg-config opencv --cflags` 
lib_gtk.o:
	$(CC) -c lib_gtk.c $(CFLAGS) $<

clean::
	rm -f *~ *.o
	rm -f projetX
# END (do not delete)


