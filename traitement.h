#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "struct.h"
#ifndef _TRAITEMENT_H
#define _TRAITEMENT_H

void set_color(IplImage *image, unsigned char* data);
void filtre_carre(IplImage *img);
void filtre_forme(IplImage *image);
int traitement();
void for_gtk(IplImage *image);
CvPoint binarisation(IplImage* image, int *nbPixels);
void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels, int fd, int a, int cint);
void getObjectColor(int event, int x, int y, int flags, void *param);
#endif 
