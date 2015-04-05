#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <stdbool.h>
#include "traitement.h"

// Maths methods
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)

// Step mooving for object min & max
#define STEP_MIN 5
#define STEP_MAX 100 

// Position of the object we overlay
CvPoint objectPos = cvPoint(-1, -1);
// Color tracked and our tolerance towards it
int h = 0, s = 0, v = 0, tolerance = 10;

IplImage *image;


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

	// Create the mask &initialize it to white (no color detected)
	mask = cvCreateImage(cvGetSize(image), image->depth, 1);

	// Create the hsv image
	hsv = cvCloneImage(image);
	cvCvtColor(image, hsv, CV_BGR2HSV);

	// We create the mask
	cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0), cvScalar(h + tolerance -1, s + tolerance, 255), mask);

	// Create kernels for the morphological operation
	kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE);

	// Morphological opening (inverse because we have white pixels on black background)
	cvDilate(mask, mask, kernel, 1);
	cvErode(mask, mask, kernel, 1);  

	// We go through the mask to look for the tracked object and get its gravity center
	for(x = 0; x < mask->width; x++) {
		for(y = 0; y < mask->height; y++) { 

			// If its a tracked pixel, count it to the center of gravity's calcul
			if(((uchar *)(mask->imageData + y*mask->widthStep))[x] == 255) {
				sommeX += x;
				sommeY += y;
				(*nbPixels)++;
			}
		}
	}

	// Show the result of the mask image
	cvShowImage("GeckoGeek Mask", mask);

	// We release the memory of kernels
	cvReleaseStructuringElement(&kernel);

	// We release the memory of the mask
	cvReleaseImage(&mask);
	// We release the memory of the hsv image
	cvReleaseImage(&hsv);

	// If there is no pixel, we return a center outside the image, else we return the center of gravity
	if(*nbPixels > 0)
		return cvPoint((int)(sommeX / (*nbPixels)), (int)(sommeY / (*nbPixels)));
	else
		return cvPoint(-1, -1);
}


/*
 * Add a circle on the video that fellow your colored object
 */
void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels) {

	int objectNextStepX, objectNextStepY;

	//	Calculate circle next position (if there is enough pixels)
	if (nbPixels > 10) {

		// Reset position if no pixel were found
		if (objectPos.x == -1 || objectPos.y == -1) {
			objectPos.x = objectNextPos.x;
			objectPos.y = objectNextPos.y;
		}

		// Move step by step the object position to the desired position
		if (abs(objectPos.x - objectNextPos.x) > STEP_MIN) {
			objectNextStepX = max(STEP_MIN, min(STEP_MAX, abs(objectPos.x - objectNextPos.x) / 2));
			objectPos.x += (-1) * sign(objectPos.x - objectNextPos.x) * objectNextStepX;
		}
		if (abs(objectPos.y - objectNextPos.y) > STEP_MIN) {
			objectNextStepY = max(STEP_MIN, min(STEP_MAX, abs(objectPos.y - objectNextPos.y) / 2));
			objectPos.y += (-1) * sign(objectPos.y - objectNextPos.y) * objectNextStepY;
		}

		// -1 = object isn't within the camera range
	} else {

		objectPos.x = -1;
		objectPos.y = -1;

	}

	// Draw an object (circle) centered on the calculated center of gravity
	if (nbPixels > 10)
		cvDrawCircle(image, objectPos, 15, CV_RGB(255, 0, 0), -1);

	cvShowImage("GeckoGeek Color Tracking", image);
}


void getObjectColor(int event, int x, int y, int flags, void *param = NULL) {

	// Vars
	CvScalar pixel;
	IplImage *hsv;

	if(event == CV_EVENT_LBUTTONUP) {

		// Get the hsv image
		hsv = cvCloneImage(image);
		cvCvtColor(image, hsv, CV_BGR2HSV);

		// Get the selected pixel
		pixel = cvGet2D(hsv, y, x);

		// Change the value of the tracked color with the color of the selected pixel
		h = (int)pixel.val[0];
		s = (int)pixel.val[1];
		v = (int)pixel.val[2];

		// Release the memory of the hsv image
		cvReleaseImage(&hsv);

	}

}


int traitement(){
	char key;
	IplImage *hsv;
	int nbPixels;
	CvPoint objectNextPos;
	CvCapture *capture = cvCreateCameraCapture( CV_CAP_ANY );  //capte image caméra
	if (!capture) {
		printf("Ouverture du flux vidéo impossible !\n");
	}else{ 
		//fait la fenêtre
		cvNamedWindow("GeckoGeek Color Tracking", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("GeckoGeek Mask", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("GeckoGeek Color Tracking", 0, 100);
		cvMoveWindow("GeckoGeek Mask", 650, 100);

		// Mouse event to select the tracked color on the original image
		cvSetMouseCallback("GeckoGeek Color Tracking", getObjectColor);

		while(key != 'q' && key != 'Q') {
			image  = cvQueryFrame(capture);
			objectNextPos = binarisation(image, &nbPixels);
			addObjectToVideo(image, objectNextPos, nbPixels);
			//affiche et attend entré clavier pendant 10ms
			key = cvWaitKey(10);
		}

		//free tout
		cvDestroyAllWindows();
		cvDestroyWindow("GeckoGeek Color Tracking");
		cvDestroyWindow("GeckoGeek Mask");
		cvReleaseCapture(&capture);
		printf("Merci d'avoir utilisé notre logiciel de capture d'image.\n");
	}
	return 0;
}

