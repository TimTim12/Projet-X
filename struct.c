#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

ColorRGB* new_color(int r,int g,int b) {
	ColorRGB *pcolor = (ColorRGB*)malloc(sizeof(struct s_ColorRGB)); 
	pcolor->r = r;
	pcolor->g = g;
	pcolor->b = b;
	return pcolor;
}

Point* new_point(int x, int y, int r, int g, int b) {
	Point *pPoint = (Point*)malloc(sizeof(struct s_Point));
	pPoint->x = x;
	pPoint->y = y;
	pPoint->color = new_color(r,g,b);
	return pPoint;
}

void print_Point(Point* p) {
	printf("coord : (%d,%d) || RGB : (%d,%d,%d)\n",p->x,p->y,p->color->r,p->color->g,p->color->b);
}

main() {
	Point *p = new_point(10,11,11,12,12);
	print_Point(p);
	return 0;
}

