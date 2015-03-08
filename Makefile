all : traitement
	gcc main.o -o main `pkg-config opencv --libs` -lm
traitement:
	gcc -c main.c -Wall `pkg-config opencv --cflags`

clean::
	rm -f *~ *.o traitement
