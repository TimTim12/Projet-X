#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>


#include "lib_gtk.h"
#include "struct.h"
#ifndef _TRAITEMENT_H
#define _TRAITEMENT_H

int get_xr();
int get_yr();
int get_xb();
int get_yb();
int get_xv();
int get_yv();

void setRecoRed(char* str);
char* getRecoRed();
void setRecoGreen(char* str);
char* getRecoGreen();
void setRecoBlue(char* str);
char* getRecoBlue();

char * get_pattern_reco();

IplImage* RGBtoHSV(const IplImage *imageRGB);
void setHSV(int h, int s, int v, int c);
void set_color(IplImage *image, unsigned char* data);
void filtre_carre(IplImage *img);
void filtre_forme(IplImage *image);
IplImage * traitement(CvCapture* capture, GdkEventKey* key);
void for_gtk(IplImage *image);
CvPoint binarisation(IplImage* image, IplImage* hsv, int *nbPixels);
void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels, int fd, int a, int cint);
void getObjectColor(int event, int x, int y, int flags, void *param);
#endif 
