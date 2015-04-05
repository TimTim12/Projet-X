#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "struct.h"

#define VECT_LEN 100

void test() {
	Point p = new_point(0,0,0,0,0);
	int x = 0;
	Mvt vect = new_vect();
	for(int i = 0; i < 100; i++) {
		if(i < 20)
			x=i;
		else if (i < 40)
			x = 2*i;
		else if (i <70)
			x = 3*i;
		else if (i<90)
			x = 2*i;
		else
			x=i;
	set_point(p,x,100,0,0,0);
	vect_update(vect,p);
	}
	printf("time=%f\n",(double)(vect[0].t -vect[1].t)/CLOCKS_PER_SEC);	
	print_vect(vect);	
}


int main() {
	test();
	return 0;
}

ColorRGB* new_color(int r,int g,int b) {
	ColorRGB *pcolor = malloc(sizeof(struct s_ColorRGB)); 
	pcolor->r = r;
	pcolor->g = g;
	pcolor->b = b;
	return pcolor;
}

void set_color(ColorRGB* c,int r, int g, int b) {
	if (c) {
		c->r = r;
		c->g = g;
		c->b = b;
	}
}

Point new_point(int x, int y, int r, int g, int b) {
	Point myPoint = malloc(sizeof(struct s_Point));
	myPoint->x = x;
	myPoint->y = y;
	myPoint->color = new_color(r,g,b);
	return myPoint;
}

void set_point(Point p, int x, int y, int r, int g, int b) {
	if (p) {
	p->x = x;
	p->y = y;
	set_color(p->color,r,g,b);
	}
}	

void printp(Point p) {
	printf("coord : (%d,%d) || RGB : (%d,%d,%d)\n",p->x,p->y,p->color->r,p->color->g,p->color->b);
}

Mvt new_vect() {
	Mvt new_vect = malloc(VECT_LEN*sizeof(struct s_Mvt));
	for(int i = 0; i < VECT_LEN; i++) {
		new_vect[i].t = 0;
		new_vect[i].v = 0;
		new_vect[i].p = new_point(0,0,0,0,0);
	}
	return new_vect;
}

void set_Mvt(Mvt new, Mvt pred,Point pt) {
	new->t = clock();
	double dy2 = pow((double)(pred->p->y - pt->y),2);
	double dx2 = pow((double)(pred->p->x - pt->x),2); 
	double dist = sqrt(dx2+dy2);
	printf("dx2= %lf - dy2= %lf - dist= %f\n",dx2,dy2,dist);
	new->v = dist / ((double)(new->t - pred->t)/CLOCKS_PER_SEC);
	*new->p = *pt;	
}

void vect_update(Mvt vect, Point p) {
	static int i = 0;
	set_Mvt(&vect[(i+1)%VECT_LEN],&vect[(i)], p);
	i = (i+1)%VECT_LEN;
	}

void print_Mvt(Mvt mvt) {
	printf("t = %f , v = %f || ", (double)mvt->t/CLOCKS_PER_SEC, mvt->v);
	Point tmp = mvt->p;
	printp(tmp);
}

void print_vect(Mvt vect) {
	for(int i = 0; i < VECT_LEN;i++) {
		printf("%2d : ",i);
		print_Mvt(&vect[i]);
	}
}

