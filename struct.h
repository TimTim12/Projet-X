#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <time.h>

//STRUCTURES
typedef struct s_Point* Point;
typedef struct s_ColorRGB ColorRGB;
typedef struct s_Mvt* Mvt;

struct s_Point {
	int x;
	int y;
	struct s_ColorRGB *color;	
};

struct s_ColorRGB {
	int r;
	int g;
	int b;
};

struct s_Mvt {
	clock_t t;
	double v;
	Point p;
};


//PROTOTYPES
ColorRGB* new_color(int r, int g, int b);
void set_color(ColorRGB* c,int r, int g, int b);
Point new_point(int x, int y, int r, int g, int b);
void set_point(Point p, int x, int y, int r, int g, int b);
void printp(Point p);
Mvt new_vect();
void set_Mvt(Mvt new, Mvt pred, Point pt);
void vect_update(Mvt vect, Point p);
void print_Mvt(Mvt mvt);
void print_vect(Mvt vect);
#endif
