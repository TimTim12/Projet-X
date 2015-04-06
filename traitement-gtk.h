#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef _TRAITEMENT_H
#define _TRAITEMENT_H

int get_xr();
int get_yr();
int get_xb();
int get_yb();
int get_xv();
int get_yv();

void set_color(IplImage *image, unsigned char* data);
void filtre_carre(IplImage *img);
void filtre_forme(IplImage *image);
int traitement();
void for_gtk(IplImage *image);
void getObjectColor(int event, int x, int y, int flags, void *param = NULL);
void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels);
CvPoint binarisation(IplImage* image, int *nbPixels);

#endif 
