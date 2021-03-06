#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <time.h>

//STRUCTURES
typedef struct s_Point* Point;
typedef struct s_ColorRGB ColorRGB;
typedef struct s_Mvt* Mvt;
typedef struct Element Element;
typedef struct linked_List linked_List;
typedef struct Template Template;

struct Template
{
	linked_List* points;
	char* name;
};

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

struct Element
{
	Point point;		// struct you must change if needed
	Element *next ;
	Element *prev;
};

struct linked_List
{
	int count;
    Element *first;
    Element *last;
};


//PROTOTYPES
void test(linked_List* mvt_list);
ColorRGB* new_color(int r, int g, int b);
void set_color2(ColorRGB* c,int r, int g, int b);
Point new_point(int x, int y, int r, int g, int b);
void set_point(Point p, int x, int y, int r, int g, int b);
void printp(Point p);
Mvt new_vect();
void set_Mvt(Mvt new_vect, Mvt pred, Point pt);
void vect_update(Mvt vect, Point p, linked_List* mvt_list);
void print_Mvt(Mvt mvt);
void print_vect(Mvt vect);


linked_List* emptyList();
void freeList(linked_List* list);
void addFirst(linked_List* list, Point p);
void addLast(linked_List* list, Point p);
Element* getIndex(linked_List* list, int index);
void addIndex(linked_List* list, int index, Point p);
void printList(linked_List* list);
#endif 
