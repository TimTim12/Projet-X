#Makefile with basic concepts

# Define compiler and options
CC=g++
CFLAGS= `pkg-config --cflags opencv gtk+-2.0`
LDFLAGS+=`pkg-config --libs opencv gtk+-2.0`


.PHONY: all clean
# avoid calling make without option
# ONLY FOR THIS EXERCICE
all: projetX

projetX: struct.o dollar1.o mouse.o lib_gtk.o traitement.o projetX.o
	$(CC) $^ -o $@ $(LDFLAGS) -lpthread -lm

clean::
	rm -f *~ *.o
	rm -f projetX

re: clean all
# END (do not delete)