all : traitement
	gcc traitement.o -o traitement `pkg-config opencv --libs` -lm
traitement:
	gcc -c traitement.c -Wall `pkg-config opencv --cflags`

clean::
	rm -f *~ *.o traitement
