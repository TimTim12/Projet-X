#ifndef _DOLLAR1
#define _DOLLAR1

#include "struct.h"

#define int_max 32767
#define SIZE 1000
#define min2(x, y) (((x) < (y)) ? (x) : (y))

char** tokenize(char* line, char* token, int sizeTab);
void printTemplate(Template* t);
void saveTemplate(linked_List* points, char* name, Template** templates);
Template** loadTemplates();
double distance(Point a, Point b);
double path_distance(linked_List* a, linked_List* b);
double path_length(linked_List *points);
linked_List *resample(linked_List *points, int n);
Point centroid(linked_List *points);
double indicative_angle(linked_List *points);
linked_List *rotate_by(linked_List *points, double w);
linked_List *scale_to(linked_List *points);
linked_List* translate_to(linked_List* points, Point k);
double distance_at_angle(linked_List* points, Template* t, double teta);
int recognize(linked_List* points, Template** templates, double* score);
linked_List* format(linked_List* points);
void printMatch(linked_List* points, Template** templates);
char* getMatch(linked_List* points, Template** templates);

#endif
