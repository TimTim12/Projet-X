#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <opencv/cv.h>
#include "struct.h"


#define VECT_LEN 100
#define MIN_VIT 1000000
#define MAX_VIT 10000000

void test(linked_List* mvt_list) {
	Point p = new_point(0,0,0,0,0);
	//int x = 0;
	Mvt vect = new_vect();
	
	for (int i =0; i < 15; i++) {
		switch (i){
			case 0:
				set_point(p,0,10,0,0,0);
				break;
			case 1:
				set_point(p,10,10,0,0,0);
				break;		
			case 2:
				set_point(p,5,40,0,0,0);
				break;
			case 3:
				set_point(p,100,100,0,0,0);
				break;
			case 4:
				set_point(p,100,150,0,0,0);
				break;
			case 5:
				set_point(p,100,200,0,0,0);
				break;		
			case 6:
				set_point(p,150,200,0,0,0);
				break;
			case 7:
				set_point(p,200,200,0,0,0);
				break;
			case 8:
				set_point(p,200,150,0,0,0);
				break;
			case 9:
				set_point(p,200,100,0,0,0);
				break;		
			case 10:
				set_point(p,150,100,0,0,0);
				break;
			case 11:
				set_point(p,100,100,0,0,0);
				break;
			case 12:
				set_point(p,100,110,0,0,0);
				break;
			case 13:
				set_point(p,120,120,0,0,0);
				break;
			case 14:
				set_point(p,125,113,0,0,0);
				break;		
			case 15:
				set_point(p,125,113,0,0,0);
				break;
		}
	

				/*for(int i = 0; i < 100; i++) {
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
		set_point(p,x,100,0,0,0)*/;
		vect_update(vect,p, mvt_list);
	}
	printf("time=%f\n",(double)(vect[0].t -vect[1].t)/CLOCKS_PER_SEC);	
	print_vect(vect);	
}

ColorRGB* new_color(int r,int g,int b) {
	ColorRGB *pcolor = (ColorRGB*)malloc(sizeof(struct s_ColorRGB)); 
	pcolor->r = r;
	pcolor->g = g;
	pcolor->b = b;
	return pcolor;
}

void set_color2(ColorRGB* c,int r, int g, int b) {
	if (c) {
		c->r = r;
		c->g = g;
		c->b = b;
	}
}

Point new_point(int x, int y, int r, int g, int b) {
	Point myPoint = (Point)malloc(sizeof(struct s_Point));
	myPoint->x = x;
	myPoint->y = y;
	myPoint->color = new_color(r,g,b);
	return myPoint;
}
void new_point1(Point p, int x, int y, int r, int g, int b) {
        Point myPoint = (Point)malloc(sizeof(struct s_Point));
        myPoint->x = x;
        myPoint->y = y;
        myPoint->color = new_color(r,g,b);
        p = myPoint;
}

void set_point(Point p, int x, int y, int r, int g, int b) {
	if (p) {
	p->x = x;
	p->y = y;
	set_color2(p->color,r,g,b);
	}
}	

void printp(Point p) {
	printf("coord : (%d,%d) || RGB : (%d,%d,%d)\n",p->x,p->y,p->color->r,p->color->g,p->color->b);
}

Mvt new_vect() {
	Mvt new_vect = (Mvt)malloc(VECT_LEN*sizeof(struct s_Mvt));
	for(int i = 0; i < VECT_LEN; i++) {
		new_vect[i].t = 0;
		new_vect[i].v = 0;
		new_vect[i].p = new_point(0,0,0,0,0);
	}
	return new_vect;
}

void set_Mvt(Mvt newM, Mvt pred,Point pt) {
	newM->t = clock();
	double dy2 = pow((double)(pred->p->y - pt->y),2);
	double dx2 = pow((double)(pred->p->x - pt->x),2); 
	double dist = sqrt(dx2+dy2);
	newM->v = dist / ((double)(newM->t - pred->t)/CLOCKS_PER_SEC);
	printf("dx2= %lf - dy2= %lf - dist= %f\n",dx2,dy2,dist);
	*newM->p = *pt;
}






///////////////////////////////////////////////////////////////////////////////////////////////////




void print_Mvt(Mvt mvt) {
	if (mvt->v > MIN_VIT /*&&  mvt->v < MAX_VIT*/) {
		printf("t = %f ,\033[32m v = %f\033[0m || ", (double)mvt->t/CLOCKS_PER_SEC, mvt->v);
	}else {
		printf("t = %f , v = %f || ", (double)mvt->t/CLOCKS_PER_SEC, mvt->v);
	}
	Point tmp = mvt->p;
	printp(tmp);
}

void print_vect(Mvt vect) {
	for(int i = 0; i < VECT_LEN;i++) 
	{
		printf("%2d : ",i);
		print_Mvt(&vect[i]);
	}
}

// LINKED LISTS

linked_List *emptyList()
{
	linked_List* list = (linked_List*)malloc(sizeof(linked_List));
	if(list == NULL)
	{
		printf("Error allocating memory !");
		exit(EXIT_FAILURE);
	}
	list->first = NULL;
	list->last = NULL;
	list->count = 0;
	return list;
}

void freeList(linked_List* list)
{
	Element* curr = list->first;
	while(curr != NULL)
	{
		Element* tmp = curr;
		curr = curr->next;
		free(tmp);
	}
	free(list);
}

void addFirst(linked_List *list, Point p)
{
	Element *elt = (Element*)malloc(sizeof(Element));
	if(list == NULL || elt == NULL)
		exit(EXIT_FAILURE);
	elt->point = p;
	elt->next = list->first;
	elt->prev = NULL;
	if(!list->count) //if empty
	{
		list->first = elt;
		list->last = elt;
	}
	else
	{
		elt->next->prev = elt;
		list->first = elt;
	}
	list->count++;
}

void addLast(linked_List *list, Point p)
{
	Element *elt = (Element*)malloc(sizeof(Element));
	if(list == NULL || elt == NULL)
		exit(EXIT_FAILURE);
	elt->point = p;
	elt->prev = list->last;
	elt->next = NULL;
	if(!list->count)
	{
		list->first = elt;
		list->last = elt;
	}
	else
	{
		elt->prev->next = elt;
		list->last = elt;
	}
	list->count++;
}

Element* getIndex(linked_List *list, int index)
{
	if(!index)
		return list->first;
	if(index == list->count-1)
		return list->last;
	if(index >= list->count || index < 0)
	{
		printf("Wrong index.\n");
		return NULL;
	}
	Element *tmp = list->first;
	int i = 0;
	while(i < index)
	{
		tmp = tmp->next;
		i++;
	}
	return tmp;
}


void addIndex(linked_List *list, int index, Point p)
{
	if(!index)
	{
		addFirst(list, p);
		return;
	}
	if(index == list->count)
	{
		addLast(list, p);
		return;
	}
	if(index >= list->count || index < 0)
	{
		printf("Wrong Index !\n");
		return;
	}
	
	Element *elt = getIndex(list, index-1);
	
	Element *tmp = (Element*)malloc(sizeof(Element));
	tmp->point = p;
	tmp->prev = elt;
	tmp->next = elt->next;	
	tmp->next->prev = tmp;
	elt->next = tmp;	
	list->count++;
}

void printList(linked_List *list)
{
	if(!list->count)
	{
		printf("List is empty\n");
		return;
	}
	printf("%d : {", list->count);
	Element *tmp = list->first;
	while(tmp != NULL && tmp != list->last)
	{
		printf("(%d,%d),",tmp->point->x, tmp->point->y);
		tmp = tmp->next;
	}
	printf("(%d,%d)}\n", tmp->point->x, tmp->point->y);
}


//returns the linked_List mvt_list filled with the mvt's points
void vect_update(Mvt vect, Point p, linked_List* mvt_list)
{
	static int i = 0;
	static int start = 0;
	static int len = 0;
	static int end = 0;
	set_Mvt(&vect[(i+1)%VECT_LEN],&vect[(i)], p);
	i = (i+1)%VECT_LEN;
	if (vect[i].v > MIN_VIT) {
		if (!start)
			start = i;
		len++;	
	}else {
		if (start && !end ) 
			end = i-1;
	} 
	//if (end > 4 || len == VECT_LEN-1) { 
		// end > 4 : Valeur arbitraire (seuil d'érreur/incohérence)
		//fin de mouvement
		if (i == 15) {
			printf("\033[33mMVT DETECTER : START=%d || END=%d\033[0m\n", start, start+len-1);
		
		
		//Sauvegarde mouvement en liste
		for(int y = start+1; y < start+len-1; y++)
		{
			addLast(mvt_list, vect[y].p);
		}
		

		

		//Fonction de traitement de mouvement, vect[start] to vect[(start+len-1)%V_L]
	//	}
		//start = len = end = 0;
	}	
}

