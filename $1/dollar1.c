#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#include "struct.c"

#define INT_MAX 32767
#define SIZE 1000
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


//////////////////////////////////
//			STEP 0				//
//////////////////////////////////

//ok
// returns a tab of strings filled with each part of line according to the give token(s)
char** tokenize(char* line, char* token, int sizeTab)
{
	int size = sizeTab;
	char** tmp = malloc(size * sizeof(char*));
	char* p = strtok(line, token);
	int count = 0;
	while(p != NULL)
	{
		if(count >= size )
		{
			size *= 2;
			char** tmp2 = realloc(tmp, size * sizeof(char*));
			tmp = tmp2;
		}
		tmp[count] = p;
		p = strtok(NULL, token);
		count++;
	}
	return tmp;
}


//ok
void printTemplate(Template* t)
{
	if(t == NULL)
		printf("Null template !\n");
	else
	{	
		printf("%s : ", t->name);
		printList(t->points);
	}
}

//ok
void saveTemplate(linked_List* points, char* name, Template** templates)
{	
	FILE* fd = fopen("templates.d1", "a");
	if(fd == NULL)
	{
		printf("Error openning templates file !\n");
		exit(EXIT_FAILURE);
	}
	fprintf(fd, "%d\n", points->count);						// write nbPoints
	fprintf(fd,	"%s", name);					// write template's name
	
	for(int i = 0; i < points->count; i++)	// write template's points
	{
		fprintf(fd, " %d %d", getIndex(points, i)->point->x, getIndex(points, i)->point->y);
	}
	fprintf(fd, "\n");
	fclose(fd);
	
	
	// Now save in templates[i]
	Template* t = malloc(sizeof(Template));
	t->name = name;
	t->points = points;
	int i = 0;
	while(templates[i] != NULL)
		i++;
	templates[i] = t;
}

//ok
Template** loadTemplates()
{
	Template** templates = malloc(250 * sizeof(Template*)); // init
	
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	int i = 0;
	
	fp = fopen("templates.d1", "ab+");
	if (fp == NULL)
	{
		printf("Error openning \"templates.d1\" !\n");
		exit(EXIT_FAILURE);
	}

	
	while(getline(&line, &len, fp) != -1) 
	{
		templates[i] = malloc(sizeof(Template));
		int nbPoints = atoi(line);
		
		templates[i]->points = emptyList();
		
		getline(&line, &len, fp);
		char** tmp = tokenize(line, " \n", 2*nbPoints + 1);
		templates[i]->name = malloc(128*sizeof(char));
		sprintf(templates[i]->name, "%s", tmp[0]);
		for(int a = 1; a < 2*nbPoints+1; a = a + 2)
		{
			addLast(templates[i]->points,new_point(atoi(tmp[a]),atoi(tmp[a+1]), 0, 0, 0));
		}
		i++;
	}
	
	return templates;
}

//ok
double distance(Point a, Point b)
{
	return sqrt(pow(a->x - b->x,2) + pow(a->y - b->y, 2));
}


double path_distance(linked_List* a, linked_List* b)
{
	double d = 0;
	for(int i = 0; i < a->count; i++)
	{
		d += distance(getIndex(a, i)->point, getIndex(b, i)->point);
	}
	return d/a->count;
}



//////////////////////////////////
//			STEP 1				//
//////////////////////////////////


//ok
double path_length(linked_List *points)
{
	double d = 0;
	for(int i = 1; i < points->count; i++)
		d += distance(getIndex(points, i-1)->point, getIndex(points, i)->point);
	return d;
}

//ok
linked_List *resample(linked_List *points, int n)
{
	double I = path_length(points) / (n-1);
	double D = 0;
	linked_List *newPoints = emptyList();
	addLast(newPoints, getIndex(points, 0)->point);
	
	for(int i = 1; i < points->count; i++)
	{
		Point pi = getIndex(points, i)->point;
		Point pi1 = getIndex(points, i-1)->point;	
		double d = distance(pi1, pi);
		if((D + d) >= I)
		{
			Point p = new_point(0,0,0,0,0);
			p->x = pi1->x + ((I-D)/d) * (pi->x - pi1->x);
			p->y = pi1->y + ((I-D)/d) * (pi->y - pi1->y);
			addLast(newPoints, p);
			addIndex(points, i, p);
			D = 0;
		}
		else
		{
			D += d;
		}
	}
	return newPoints;
}



//////////////////////////////////
//			STEP 2				//
//////////////////////////////////


//ok
Point centroid(linked_List *points)
{
	int x = 0, y = 0;
	for(int i = 0; i < points->count; i++)
	{
		Point tmp = getIndex(points, i)->point;
		x += tmp->x;
		y += tmp->y;
	}
	return new_point(x/points->count,y/points->count,0,0,0);
}

// NOT SURE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
double indicative_angle(linked_List *points)
{
	Point c = centroid(points);
 	return (double) atan2(c->y - getIndex(points, 0)->point->y, c->x - getIndex(points, 0)->point->x);
}

//ok
linked_List *rotate_by(linked_List *points, double w)
{
	double angleRad = w;
	Point c = centroid(points);
	linked_List *newPoints = emptyList();
	for(int i = 0; i < points->count; i++)
	{
		Point p = getIndex(points, i)->point;
		int x = (p->x - c->x)*cos(angleRad) - (p->y - c->y)*sin(angleRad);// + c->x;  
		int y = (p->x - c->x)*sin(angleRad) + (p->y - c->y)*cos(angleRad);// + c->y;  
		addLast(newPoints, new_point(x, y, 0, 0, 0));
	}
	return newPoints;
}


//////////////////////////////////
//			STEP 3				//
//////////////////////////////////


//ok
linked_List *scale_to(linked_List *points)
{
	//bounded box
	int minX = INT_MAX, minY = INT_MAX, maxX = 0, maxY = 0;
	for(int i = 0; i < points->count; i++)
	{
		Point p = getIndex(points, i)->point;
		if(p->x < minX)
			minX = p->x;
		if(p->x > maxX)
			maxX = p->x;
		if(p->y < minY)
			minY = p->y;
		if(p->y > maxY)
			maxY = p->y;
	}
	
	int width = maxX - minX;
	int height = maxY - minY;
	linked_List *newPoints = emptyList();
	
	for(int i = 0; i < points->count; i++)
	{
		Point tmp = getIndex(points, i)->point;
		addLast(newPoints, new_point(tmp->x * SIZE / width, tmp->y * SIZE / height, 0, 0, 0));
	}
	return newPoints;
}

linked_List* translate_to(linked_List* points, Point k)
{
	Point c = centroid(points);
	linked_List* newPoints = emptyList();
	for(int i = 0; i < points->count; i++)
	{
		Point tmp = getIndex(points, i)->point;
		addLast(newPoints, new_point(tmp->x + k->x - c->x, tmp->y + k->y - c->y, 0, 0, 0));
	}
	return newPoints;
}


//////////////////////////////////
//			STEP 4				//
//////////////////////////////////


double distance_at_angle(linked_List* points, Template* t, double teta)
{
	return path_distance(rotate_by(points, M_PI*teta/180), t->points); //teta to radiant
}

/*double distance_at_best_angle(linked_List* points, Template* t, int Oa, int Ob, int Oc)     // FORGOTTEN FOR S2
{
	double phi = 0.5*(-1 + sqrt(5));
	
	double x1 = phi*Oa + (1-phi)*Ob;
	double f1 = distance_at_angle(points, t, x1);
	
	double x2 = (1-phi)*Oa + phi*Ob;
	double f2 = distance_at_angle(points, t, x2);
	
	//printf("x1 = %lf\tx2 = %lf\nf1 = %lf\tf2 = %lf\n", x1, x2, f1, f2);
	while(fabs(Ob - Oa) > Oc)
	{
		if(f1 < f2)
		{
			Ob = x2;
			x2 = x1;
			f2 = f1;
			x1 = phi*Oa + (1-phi)*Ob;
			f1 = distance_at_angle(points, t, x1);
		}
 		else
		{
			Oa = x1;
			x1 = x2;
			f1 = f2;
			x2 = (1 - phi)*Oa + phi*Ob;
			f2 = distance_at_angle(points, t, x2);
		}
	}
	if(f1 < f2)
		return f1;
	return f2;
}*/

// return number of the template matched in templates[] or -1 if theshold not reached
int recognize(linked_List* points, Template** templates, double* score)
{
	double b = INT_MAX;
	int i = 0, r =-1;
	while(templates[i] != NULL)
	{
		double d = 0; //distance_at_best_angle(points, templates[i], -45, +45, 2);
		for(int j = 0; j < MIN(points->count,templates[i]->points->count); j++)			//use of min to bypass the resample bug
		{
			d += distance(getIndex(points, j)->point, getIndex(templates[i]->points, j)->point);
		}
		d = d/points->count;
		
		if(d<b)
		{
			r = i;
			b = d;			
		}
		i++;
	}
	
	
	*score = (1 - (b/(0.5*sqrt(SIZE*SIZE + SIZE*SIZE)))) * 100;
	if(*score > 66.0)
		return r;
	else
		return -1;
}



/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



linked_List* format(linked_List* points)
{
	linked_List* rPoints = resample(points, 32);
	double angle = indicative_angle(rPoints);
	return scale_to(rotate_by(rPoints, -angle));
}

linked_List* getSquare()
{
	linked_List* points = emptyList();
 	addLast(points, new_point(100,100,0,0,0));
	addLast(points, new_point(100,150,0,0,0));
 	addLast(points, new_point(100,200,0,0,0));
	addLast(points, new_point(150,200,0,0,0));
 	addLast(points, new_point(200,200,0,0,0));
	addLast(points, new_point(200,150,0,0,0));
 	addLast(points, new_point(200,100,0,0,0));
	addLast(points, new_point(150,100,0,0,0));
	return points;
}

linked_List* getCircle()
{
	linked_List* points = emptyList();
	addLast(points, new_point(0,400,0,0,0));
	addLast(points, new_point(133,370,0,0,0));
	addLast(points, new_point(300,300,0,0,0));
	addLast(points, new_point(350,250,0,0,0));
	addLast(points, new_point(400,0,0,0,0));
	addLast(points, new_point(380,-100,0,0,0));
	addLast(points, new_point(300,-300,0,0,0));
	addLast(points, new_point(140,-380,0,0,0));
	addLast(points, new_point(0,-400,0,0,0));
	addLast(points, new_point(-160,-385,0,0,0));
	addLast(points, new_point(-300,-300,0,0,0));
	addLast(points, new_point(-375,-250,0,0,0));
	addLast(points, new_point(-400,0,0,0,0));
	addLast(points, new_point(-390,155,0,0,0));
	addLast(points, new_point(-300,300,0,0,0));
	addLast(points, new_point(-155,390,0,0,0));
	return points;
}

linked_List* getTriangle()
{
	linked_List* points = emptyList();
	addLast(points, new_point(300,300,0,0,0));
	addLast(points, new_point(250,250,0,0,0));
	addLast(points, new_point(200,200,0,0,0));
	addLast(points, new_point(150,150,0,0,0));
	addLast(points, new_point(100,100,0,0,0));
	addLast(points, new_point(200,100,0,0,0));
	addLast(points, new_point(300,100,0,0,0));
	addLast(points, new_point(400,100,0,0,0));
	addLast(points, new_point(500,100,0,0,0));
	addLast(points, new_point(450,150,0,0,0));
	addLast(points, new_point(400,200,0,0,0));
	addLast(points, new_point(350,250,0,0,0));
	return points;
}

linked_List* getmTriangle()
{
	linked_List* points = emptyList();
	addLast(points, new_point(290,300,0,0,0));
	addLast(points, new_point(250,220,0,0,0));
	addLast(points, new_point(200,200,0,0,0));
	addLast(points, new_point(150,150,0,0,0));
	addLast(points, new_point(100,100,0,0,0));
	addLast(points, new_point(200,90,0,0,0));
	addLast(points, new_point(300,105,0,0,0));
	addLast(points, new_point(400,110,0,0,0));
	addLast(points, new_point(500,100,0,0,0));
	addLast(points, new_point(440,160,0,0,0));
	addLast(points, new_point(400,200,0,0,0));
	addLast(points, new_point(350,250,0,0,0));
	return points;
}

void saveStar(Template** templates)
{
	linked_List* star = emptyList();
	addLast(star, new_point(300,600,0,0,0));
	addLast(star, new_point(400,400,0,0,0));
	addLast(star, new_point(600,400,0,0,0));
	addLast(star, new_point(450,250,0,0,0));
	addLast(star, new_point(500,0,0,0,0));
	addLast(star, new_point(300,200,0,0,0));
	addLast(star, new_point(100,0,0,0,0));
	addLast(star, new_point(150,250,0,0,0));
	addLast(star, new_point(0,400,0,0,0));
	addLast(star, new_point(200,400,0,0,0));
	saveTemplate(format(star), "star", templates);
}

void printMatch(linked_List* points, Template** templates)
{
	double score;
	int result = recognize(points, templates, &score);
	if(result != -1)
		printf("match with %s with %lf percent\n",templates[result]->name ,score);		// print index recognized
	else
		printf("Pattern didn't match with any known form.\n");
}


void step_by_step(Template** templates)
{
	printf("Templates successfully loaded !\n");
	
	getchar();
	
	printf("Displaying Step-By-Step format.\n");
	
	getchar();
	
	linked_List* Circle = getCircle();
	printf("Original circle : ");
	printList(Circle);
	
	getchar();
	
	Circle = resample(Circle, 32);
	printf("Resampled circle : ");
	printList(Circle);
	
	getchar();
	
	double angle = indicative_angle(Circle);
	Circle = rotate_by(Circle, -angle);
	printf("Rotated circle : ");
	printList(Circle);
	
	getchar();
	
	Circle = scale_to(Circle);
	printf("Scaled to 1000² box : ");
	printList(Circle);
	
	getchar();
	
	linked_List* Square = getSquare();
	linked_List* Triangle = getTriangle();
	saveTemplate(Circle, "Circle", templates);
	saveTemplate(format(Square), "Square", templates);
	saveTemplate(format(Triangle), "Triangle", templates);
	printf("Saved Circle, Square and Triangle !\nDisplaying saved templates :\n");
	int i = 0;
	while(templates[i] != NULL)
	{
		printTemplate(templates[i]);
		i++;
	}
	
	getchar();
	
	printf("Sending the previous Circle to the recognizer : \n");
	printMatch(Circle, templates);
	
	getchar();
	
	printf("Sending the previous Triangle modified to the recognizer : \n");	
	printMatch(format(getmTriangle()), templates);
	
	getchar();
	
	printf("Sending a random form to the recognizer : \n");
	linked_List* line = emptyList();
	addLast(line, new_point(249,458,0,0,0));
	addLast(line, new_point(32,652,0,0,0));
	addLast(line, new_point(680,-919,0,0,0));
	addLast(line, new_point(1016,1,0,0,0));
	addLast(line, new_point(546,-651,0,0,0));
	addLast(line, new_point(-195,787,0,0,0));
	addLast(line, new_point(-64,-1,0,0,0));
	printList(line);
	line = format(line);
	printMatch(line, templates);
}


int main() 
{
	test();
	
	getchar();
	
	printf("\033[1;1H\033[2J"); //clear console
	
 	Template** templates = loadTemplates();
 	saveStar(templates);
	step_by_step(templates);
	return 0;
}