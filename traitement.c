#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "traitement.h"
#include <string.h>
#include <math.h>
#include "mouse.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)


#define STEP_MIN 10
#define STEP_MAX 100 

// Position de l'objet, "couleur", seuil de tolérance
CvPoint objectPos[3] = {cvPoint(-1, -1), cvPoint(-1,-1), cvPoint(-1,-1)};
int h[3] = {0,0,0}, s[3] = {0,0,0}, v[3] = {0,0,0};
int tolerance = 10, fd, cint = 0;

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

IplImage* RGBtoHSV(const IplImage *imageRGB)
{
	float fR, fG, fB;
	float fH, fS, fV;
	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

	// Create a blank HSV image
	IplImage *imageHSV = cvCreateImage(cvGetSize(imageRGB), 8, 3);
	if (!imageHSV || imageRGB->depth != 8 || imageRGB->nChannels != 3) {
		printf("ERROR in convertImageRGBtoHSV()! Bad input image.\n");
		exit(1);
	}

	int h = imageRGB->height;		// Pixel height.
	int w = imageRGB->width;		// Pixel width.
	int rowSizeRGB = imageRGB->widthStep;	// Size of row in bytes, including extra padding.
	char *imRGB = imageRGB->imageData;	// Pointer to the start of the image pixels.
	int rowSizeHSV = imageHSV->widthStep;	// Size of row in bytes, including extra padding.
	char *imHSV = imageHSV->imageData;	// Pointer to the start of the image pixels.
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			// Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
			uchar *pRGB = (uchar*)(imRGB + y*rowSizeRGB + x*3);
			int bB = *(uchar*)(pRGB+0);	// Blue component
			int bG = *(uchar*)(pRGB+1);	// Green component
			int bR = *(uchar*)(pRGB+2);	// Red component

			// Convert from 8-bit integers to floats.
			fR = bR * BYTE_TO_FLOAT;
			fG = bG * BYTE_TO_FLOAT;
			fB = bB * BYTE_TO_FLOAT;

			// Convert from RGB to HSV, using float ranges 0.0 to 1.0.
			float fDelta;
			float fMin, fMax;
			int iMax;
			// Get the min and max, but use integer comparisons for slight speedup.
			if (bB < bG) {
				if (bB < bR) {
					fMin = fB;
					if (bR > bG) {
						iMax = bR;
						fMax = fR;
					}
					else {
						iMax = bG;
						fMax = fG;
					}
				}
				else {
					fMin = fR;
					fMax = fG;
					iMax = bG;
				}
			}
			else {
				if (bG < bR) {
					fMin = fG;
					if (bB > bR) {
						fMax = fB;
						iMax = bB;
					}
					else {
						fMax = fR;
						iMax = bR;
					}
				}
				else {
					fMin = fR;
					fMax = fB;
					iMax = bB;
				}
			}
			fDelta = fMax - fMin;
			fV = fMax;				// Value (Brightness).
			if (iMax != 0) {			// Make sure it's not pure black.
				fS = fDelta / fMax;		// Saturation.
				float ANGLE_TO_UNIT = 1.0f / (6.0f * fDelta);	// Make the Hues between 0.0 to 1.0 instead of 6.0
				if (iMax == bR) {		// between yellow and magenta.
					fH = (fG - fB) * ANGLE_TO_UNIT;
				}
				else if (iMax == bG) {		// between cyan and yellow.
					fH = (2.0f/6.0f) + ( fB - fR ) * ANGLE_TO_UNIT;
				}
				else {				// between magenta and cyan.
					fH = (4.0f/6.0f) + ( fR - fG ) * ANGLE_TO_UNIT;
				}
				// Wrap outlier Hues around the circle.
				if (fH < 0.0f)
					fH += 1.0f;
				if (fH >= 1.0f)
					fH -= 1.0f;
			}
			else {
				// color is pure Black.
				fS = 0;
				fH = 0;	// undefined hue
			}

			// Convert from floats to 8-bit integers.
			int bH = (int)(0.5f + fH * 255.0f);
			int bS = (int)(0.5f + fS * 255.0f);
			int bV = (int)(0.5f + fV * 255.0f);

			// Clip the values to make sure it fits within the 8bits.
			if (bH > 255)
				bH = 255;
			if (bH < 0)
				bH = 0;
			if (bS > 255)
				bS = 255;
			if (bS < 0)
				bS = 0;
			if (bV > 255)
				bV = 255;
			if (bV < 0)
				bV = 0;

			// Set the HSV pixel components.
			uchar *pHSV = (uchar*)(imHSV + y*rowSizeHSV + x*3);
			*(pHSV+0) = bH;		// H component
			*(pHSV+1) = bS;		// S component
			*(pHSV+2) = bV;		// V component
		}
	}
	return imageHSV;
}


CvPoint binarisation(IplImage* image, int *nbPixels, int cint) {

	int x, y;
	CvScalar pixel;
	IplImage *hsv, *mask;
	IplConvKernel *kernel;
	int sommeX = 0, sommeY = 0;
	*nbPixels = 0;

	// création du masque blanc
	mask = cvCreateImage(cvGetSize(image), image->depth, 1);

	// image (Hue, Saturation, Value, en français : TSV – Teinte, Saturation, Valeur)
	//hsv = cvCloneImage(image);
	//cvCvtColor(image, hsv, CV_BGR2HSV);
	hsv = RGBtoHSV(image);

	//mets à jour le masque
	cvInRangeS(hsv, cvScalar(h[cint] - tolerance -1, s[cint] - tolerance, 0), cvScalar(h[cint] + tolerance -1, s[cint] + tolerance, 255), mask);

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



void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels, int fd, int a,int cint) {
	//  Point p = NULL;
	//  new_point1(p,0,0,0,0,0);
	int objectNextStepX, objectNextStepY;

	if (nbPixels > 10) {

		// Reset position 
		if (objectPos[cint].x == -1 || objectPos[cint].y == -1) {
			objectPos[cint].x = objectNextPos.x;
			objectPos[cint].y = objectNextPos.y;
		}

		//lissage du déplacement 
		if (abs(objectPos[cint].x - objectNextPos.x) > STEP_MIN) {
			objectNextStepX = max(STEP_MIN, min(STEP_MAX, abs(objectPos[cint].x - objectNextPos.x) / 2));
			objectPos[cint].x += (-1) * sign(objectPos[cint].x - objectNextPos.x) * objectNextStepX;
		}
		if (abs(objectPos[cint].y - objectNextPos.y) > STEP_MIN) {
			objectNextStepY = max(STEP_MIN, min(STEP_MAX, abs(objectPos[cint].y - objectNextPos.y) / 2));
			objectPos[cint].y += (-1) * sign(objectPos[cint].y - objectNextPos.y) * objectNextStepY;
		}

		// -1 = objet hors caméra
	} else {

		objectPos[cint].x = -1;
		objectPos[cint].y = -1;
	}   
	if(a>0)
		set_coord_mouse(fd, -(objectPos[cint].x* 1366)/620, (objectPos[cint].y* 768)/480);
	//set_coord_mouse(fd, -objectPos.x* 2, objectPos.y* 2);
	//Dessine moi un mouton	
	if (nbPixels > 10)
		cvDrawCircle(image, objectPos[cint], 5, CV_RGB(255, 0, 0), -1);
	cvShowImage("Color Tracking", image);
}


void getObjectColor(int event, int x, int y, int flags, void *param) {

	CvScalar pixel;
	IplImage *hsv;
	//fd = connect_mouse("/dev/input/event12");
	if(event == CV_EVENT_LBUTTONUP) {

		//hsv = cvCloneImage(image2);
		//cvCvtColor(image2, hsv, CV_BGR2HSV);
		hsv = RGBtoHSV(image2);
		pixel = cvGet2D(hsv, y, x);
		
		// Mets à jour la couleur rechercher
		h[cint] = (int)pixel.val[0];
		s[cint] = (int)pixel.val[1];
		v[cint] = (int)pixel.val[2];
		
		printf("-------");
		for (int c=0; c <3; c++) {
		printf("\nh(%d):%d",c, h[c]);
		printf("\ns(%d):%d",c, s[c]);
		printf("\nv(%d):%d\n",c, v[c]);
		}


		/*h[] = (int)pixel.val[0];
		s[] = (int)pixel.val[1];
		v[] = (int)pixel.val[2];

		h[] = (int)pixel.val[0];
		s[] = (int)pixel.val[1];
		v[] = (int)pixel.val[2];
*/
		cvReleaseImage(&hsv);
	}

}


int traitement(){
	//Point p;
	//p = new_point(0,0,0,0,0);
	char key;
	IplImage *hsv;
	int nbPixels[3];
	CvPoint oNPR,oNPG, oNPB ; //objectNextPos
	CvCapture *capture = cvCreateCameraCapture( CV_CAP_ANY );  //capte image caméra
	if (!capture) {
		printf("Ouverture du flux vidéo impossible !\n");
	}else{ 
		//fait les fenêtre

		cvNamedWindow("Color Tracking", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Mask1", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("Color Tracking", 0, 100);
		cvMoveWindow("Mask", 650, 100);

		// clique souris
		cvSetMouseCallback("Color Tracking", getObjectColor);
		int a = 0;
		//int fd = connect_mouse("/dev/input/event12");
		while(key != 'q' && key != 'Q') {
			image2  = cvQueryFrame(capture);
			if(key == 'r')
				cint = 0;
			if(key == 'g')
				cint = 1;
			if(key == 'b')
				cint = 2;

			oNPR = binarisation(image2, &nbPixels[0], 0);
			oNPG = binarisation(image2, &nbPixels[1], 1);
			oNPB = binarisation(image2, &nbPixels[2], 2);
			if( key == 's') 
			{ fd = connect_mouse("/dev/input/event14");a = 1;}
			else
			addObjectToVideo(image2, oNPR, nbPixels[0], fd, a, 0);
			addObjectToVideo(image2, oNPG, nbPixels[1], fd, a, 1);
			addObjectToVideo(image2, oNPB, nbPixels[2], fd, a, 2);
			//affiche et attend entré clavier pendant 10ms
			key = cvWaitKey(10);
		}
		close(fd);
		//free tout
		printf("\nimage de taille : %dx%d\n", image2->width, image2->height);

		cvDestroyAllWindows();
		cvDestroyWindow("Color Tracking");
		cvDestroyWindow("Mask");
		cvReleaseCapture(&capture);
		printf("Merci d'avoir utilisé notre logiciel de capture d'image.\n");
	}
	return 0;
}


/*void for_gtk(IplImage *image){
	int nbPixels;
	//Point p;
	//p = new_point(0,0,0,0,0);
	CvPoint objectNextPos;
	cvSetMouseCallback("Color Tracking", getObjectColor);
	objectNextPos = binarisation(image, &nbPixels);
	addObjectToVideo(image, objectNextPos, nbPixels);
}*/
