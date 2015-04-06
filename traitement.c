#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "traitement.h"
#include "struct.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)


#define STEP_MIN 10
#define STEP_MAX 100 

// Position de l'objet, "couleur", seuil de tolérance
CvPoint objectPos = cvPoint(-1, -1);
int h = 0, s = 0, v = 0, tolerance = 10;

IplImage *image2;


int xr=0,yr=0,xb=0,yb=0,xv=0,yv=0;

int get_xr() {
	    return xr;
}
int get_yr() {
	    return yr;
}
int get_xb() {
	    return xb;
}
int get_yb() {
	    return yb;
}
int get_xv() {
	    return xv;
}
int get_yv() {
	    return yv;
}



CvPoint binarisation(IplImage* image, int *nbPixels) {

	int x, y;
	CvScalar pixel;
	IplImage *hsv, *mask;
	IplConvKernel *kernel;
	int sommeX = 0, sommeY = 0;
	*nbPixels = 0;

	// création du masque blanc
	mask = cvCreateImage(cvGetSize(image), image->depth, 1);

	// image (Hue, Saturation, Value, en français : TSV – Teinte, Saturation, Valeur)
	hsv = cvCloneImage(image);
	cvCvtColor(image, hsv, CV_BGR2HSV);
	
	//mets à jour le masque
	cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0), cvScalar(h + tolerance -1, s + tolerance, 255), mask);

	//élimination des pixel isolé 
	kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE);
	cvDilate(mask, mask, kernel, 1);
	cvErode(mask, mask, kernel, 1);  

	// calcule du centre de l'objet que l'on veut
	for(x = 0; x < mask->width; x++) {
		for(y = 0; y < mask->height; y++) {
			if(((uchar *)(mask->imageData + y*mask->widthStep))[x] == 255) {
				sommeX += x;
				sommeY += y;
				(*nbPixels)++;
			}
		}
	}

	
	cvShowImage("Mask", mask);

	//free tout 
	cvReleaseStructuringElement(&kernel);
	cvReleaseImage(&mask);
	cvReleaseImage(&hsv);

	//si pas d'objet dans la vidéo, point hors champ 
	if(*nbPixels > 20)
		return cvPoint((int)(sommeX / (*nbPixels)), (int)(sommeY / (*nbPixels)));
	else
		return cvPoint(-1, -1);
}





void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels) {

	int objectNextStepX, objectNextStepY;

	if (nbPixels > 10) {

		// Reset position 
		if (objectPos.x == -1 || objectPos.y == -1) {
			objectPos.x = objectNextPos.x;
			objectPos.y = objectNextPos.y;
		}

		//lissage du déplacement 
		if (abs(objectPos.x - objectNextPos.x) > STEP_MIN) {
			objectNextStepX = max(STEP_MIN, min(STEP_MAX, abs(objectPos.x - objectNextPos.x) / 2));
			objectPos.x += (-1) * sign(objectPos.x - objectNextPos.x) * objectNextStepX;
		}
		if (abs(objectPos.y - objectNextPos.y) > STEP_MIN) {
			objectNextStepY = max(STEP_MIN, min(STEP_MAX, abs(objectPos.y - objectNextPos.y) / 2));
			objectPos.y += (-1) * sign(objectPos.y - objectNextPos.y) * objectNextStepY;
		}

		// -1 = objet hors caméra
	} else {

		objectPos.x = -1;
		objectPos.y = -1;
	}

	//Dessine moi un mouton	
	if (nbPixels > 10)
		cvDrawCircle(image, objectPos, 5, CV_RGB(255, 0, 0), -1);
	cvShowImage("Color Tracking", image);
}


void getObjectColor(int event, int x, int y, int flags, void *param) {

	CvScalar pixel;
	IplImage *hsv;

	if(event == CV_EVENT_LBUTTONUP) {

		hsv = cvCloneImage(image2);
		cvCvtColor(image2, hsv, CV_BGR2HSV);
		pixel = cvGet2D(hsv, y, x);

		// Mets à jour la couleur rechercher
		h = (int)pixel.val[0];
		s = (int)pixel.val[1];
		v = (int)pixel.val[2];

		cvReleaseImage(&hsv);
	}

}


int traitement(){
	//Point p;
	//p = new_point(0,0,0,0,0);
	char key;
	IplImage *hsv;
	int nbPixels;
	CvPoint objectNextPos;
	CvCapture *capture = cvCreateCameraCapture( CV_CAP_ANY );  //capte image caméra
	if (!capture) {
		printf("Ouverture du flux vidéo impossible !\n");
	}else{ 
		//fait les fenêtre

		cvNamedWindow("Color Tracking", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Mask", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("Color Tracking", 0, 100);
		cvMoveWindow("Mask", 650, 100);

		// clique souris
		cvSetMouseCallback("Color Tracking", getObjectColor);

		while(key != 'q' && key != 'Q') {
			image2  = cvQueryFrame(capture);
			objectNextPos = binarisation(image2, &nbPixels);
			addObjectToVideo(image2, objectNextPos, nbPixels);
			//affiche et attend entré clavier pendant 10ms
			key = cvWaitKey(10);
		}

		//free tout
		cvDestroyAllWindows();
		cvDestroyWindow("Color Tracking");
		cvDestroyWindow("Mask");
		cvReleaseCapture(&capture);
		printf("Merci d'avoir utilisé notre logiciel de capture d'image.\n");
	}
	return 0;
}


void for_gtk(IplImage *image){
	int nbPixels;
	//Point p;
	//p = new_point(0,0,0,0,0);
	CvPoint objectNextPos;
	cvSetMouseCallback("Color Tracking", getObjectColor);
	objectNextPos = binarisation(image, &nbPixels);
	addObjectToVideo(image, objectNextPos, nbPixels);
}
