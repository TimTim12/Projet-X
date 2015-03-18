#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <stdbool.h>
#include "traitement.h"

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

void etalonage(char color, int value, IplImage *image, unsigned char* data){
	int i, j;
	int r=0,v=0,b=0;
	int pix1, pix2, pix3;
	if(color == 'r') {
		r=255;
		pix1 = 2;
		pix2 = 0;
		pix3 = 1;
	}else if (color == 'v'){
		v=255;
		pix1 = 1;
		pix2 = 0;
		pix3 = 2;
	}else { 
		b=255;
		pix1 = 0;
		pix2 = 1;
		pix3 = 2;
	}
	for (i = 0; i < image->height; i++){
		for (j = 0; j < image->width;j++){
			if (data[pix1+3*j+3*image->width*i] > value && data[pix2+3*j+3*image->width*i] < 130 && data[pix3+3*j+3*image->width*i] < 130){
				data[0+3*j+3*image->width*i] = b; //bleu
				data[1+3*j+3*image->width*i] = v; //vert
				data[2+3*j+3*image->width*i] = r; //rouge
			}
		}
	}
}


void set_color(char color, IplImage *image, unsigned char* data){
	int i, j;
	if (color != 'a') {
		etalonage(color, 100, image, data);
	}else {
		for (i = 0; i < image->height; i++){
			for (j = 0; j < image->width;j++){
				if (data[2+3*j+3*image->width*i] > 160 && data[0+3*j+3*image->width*i] < 100 && data[1+3*j+3*image->width*i] < 100){
					data[0+3*j+3*image->width*i] = 0 ; //bleu
					data[1+3*j+3*image->width*i] = 0; //vert
					data[2+3*j+3*image->width*i] = 255; //rouge
				}
				if (data[2+3*j+3*image->width*i] < 100 && data[0+3*j+3*image->width*i] > 160 && data[1+3*j+3*image->width*i] < 100){
					data[0+3*j+3*image->width*i] = 255 ; //bleu
					data[1+3*j+3*image->width*i] = 0; //vert
					data[2+3*j+3*image->width*i] = 0; //rouge
				}
				if (data[2+3*j+3*image->width*i] <60 && data[0+3*j+3*image->width*i] < 60 && data[1+3*j+3*image->width*i] > 80){
					data[0+3*j+3*image->width*i] = 0 ; //bleu
					data[1+3*j+3*image->width*i] = 255; //vert
					data[2+3*j+3*image->width*i] = 0; //rouge
				}
			}
		}
	}
}


void filtre_carre(IplImage *img){
	int i, j, h, w;
	unsigned char* data = (unsigned char*)(img->imageData);
	set_color('a', img, data);
	for (i = 0; i < img->height; i++){   //set les pixels
		for (j = 0; j < img->width;j++){
			if (data[2+3*j+3*img->width*i] > 200 && data[0+3*j+3*img->width*i] < 100 && data[1+3*j+3*img->width*i] < 100){
				data[0+3*j+3*img->width*i] = 0; //bleu
				data[1+3*j+3*img->width*i] = 0; //vert
				data[2+3*j+3*img->width*i] = 255; //rouge
				w = j - 40;
				if (w > 0 && data[2+3*w+3*img->width*i] != 255 ){
					for(h = i - 20; h < i + 20; h++){
						if (h > 0 && h < img->height ){
							data[0+3*w+3*img->width*h] = 0; 
							data[1+3*w+3*img->width*h] = 255; 
							data[2+3*w+3*img->width*h] = 0; 
						}
					}
				}
				w = j + 40;
				if (w < img->width && data[2+3*w+3*img->width*i] != 255 ){
					for(h = i - 20; h < i + 20; h++){
						if (h > 0 && h < img->height ){
							data[0+3*w+3*img->width*h] = 0;
							data[1+3*w+3*img->width*h] = 255; 
							data[2+3*w+3*img->width*h] = 0;
						}
					}
				}
				h = i - 40;
				if (h > 0 && data[2+3*j+3*img->width*h] != 255 ){
					for(w = j - 20; w < j + 20; w++){
						if (w > 0 && w < img->height ){
							data[0+3*w+3*img->width*h] = 0; 
							data[1+3*w+3*img->width*h] = 255; 
							data[2+3*w+3*img->width*h] = 0; 
						}
					}
				}
				h = i + 40;
				if (h < img->height && data[2+3*j+3*img->width*h] != 255 ){
					for(w = j - 20; w < j + 20; w++){
						if (w > 0 && w < img->height ){
							data[0+3*w+3*img->width*h] = 0; 
							data[1+3*w+3*img->width*h] = 255; 
							data[2+3*w+3*img->width*h] = 0;
						}
					}
				}
			}
		}
	}
}
void filtre_forme(IplImage *image){
	int i = 0, j = 0, h = 0, w = 0, y = 0, z = 0;
	bool tour;
	int moy_xr=0, moy_yr=0, moy_xv=0, moy_yv=0, moy_xb=0,moy_yb=0;
	int nb_r=0,nb_v=0,nb_b=0;
	unsigned char* data = (unsigned char*)(image->imageData);
	set_color('a', image, data);				
	//set les carré
	for (i = 0; i < image->height; i++){
		for (j = 0; j < image->width; j++){
			tour = false;
			//regarde si les pixel autour sont rouge
			if (data[2+3*j+3*image->width*i] == 255){	
				tour = true;
				for(y = i-5; y <= i+5; y++){
					for(z = j+5; z <= j+5; z++){
						if(y > 0 && y < image->height && z > 0 && z < image->width){
							if(data[2+3*z+3*image->width*y] != 255 || data[0+3*z+3*image->width*y] != 0 || data[1+3*z+3*image->width*y] != 0)
								tour = false;
							if(!tour) break;
						}
						if(!tour) break;
					}
				}
				if(tour){
					for(h = i-10; h <= i+10; h++){
						for( w= j-10; w <= j+10; w++){
							if(h > 0 && h < image->height && w > 0 && w < image->width && data[2+3*w+3*image->width*h] != 255){
								if((h == i-10 || h == i+10) || (w == i+10 || w == j+10)){
									data[0+3*w+3*image->width*h] = 0; 
									data[1+3*w+3*image->width*h] = 240; 
									data[2+3*w+3*image->width*h] = 240;
									moy_xr +=w;
									moy_yr +=h;
									nb_r++;
								}
							}
						}
					}
				}
			}
			//Pixel bleu ?
			if (data[0+3*j+3*image->width*i] == 255){	
				tour = true;
				for(y = i-5; y <= i+5; y++){
					for(z = j+5; z <= j+5; z++){
						if(y > 0 && y < image->height && z > 0 && z < image->width){
							if(data[0+3*z+3*image->width*y] != 255 || data[2+3*z+3*image->width*y] != 0 || data[1+3*z+3*image->width*y] != 0)
								tour = false;
							if(!tour) break;
						}
						if(!tour) break;
					}
				}
				if(tour){
					for(h = i-10; h <= i+10; h++){
						for( w= j-10; w <= j+10; w++){
							if(h > 0 && h < image->height && w > 0 && w < image->width && data[0+3*w+3*image->width*h] != 255){
								if((h == i-10 || h == i+10) || (w == i+10 || w == j+10)){
									data[0+3*w+3*image->width*h] = 0; 
									data[1+3*w+3*image->width*h] = 240; 
									data[2+3*w+3*image->width*h] = 240;
									moy_xb +=w;
									moy_yb +=h;
									nb_b++;
		
								}
							}
						}
					}
				}
			}//Pixel vert ?
			if (data[1+3*j+3*image->width*i] == 255){	
				tour = true;
				for(y = i-5; y <= i+5; y++){
					for(z = j+5; z <= j+5; z++){
						if(y > 0 && y < image->height && z > 0 && z < image->width){
							if(data[1+3*z+3*image->width*y] != 255 || data[0+3*z+3*image->width*y] != 0 || data[2+3*z+3*image->width*y] != 0)
								tour = false;
							if(!tour) break;
						}
						if(!tour) break;
					}
				}
				if(tour){
					for(h = i-10; h <= i+10; h++){
						for( w= j-10; w <= j+10; w++){
							if(h > 0 && h < image->height && w > 0 && w < image->width && data[1+3*w+3*image->width*h] != 255){
								if((h == i-10 || h == i+10) || (w == i+10 || w == j+10)){
									data[0+3*w+3*image->width*h] = 0; 
									data[1+3*w+3*image->width*h] = 240; 
									data[2+3*w+3*image->width*h] = 240;
									moy_xv +=w;
									moy_yv +=h;
									nb_v++;
		
								}
							}
						}
					}
				}
			}
		}
	}
	xb = yb = xv = yv = xr = yr = 0;
	for (int i = -5; i < 6; i++) {
		for (int j = -5; j < 6; j++) {
			if (nb_b > 0) {
				xb = moy_xb/nb_b;
				yb = moy_yb/nb_b;
				data[0+3*(i+(moy_xb/nb_b))+3*image->width*(j+(moy_yb/nb_b))]= 240;
				data[1+3*(i+(moy_xb/nb_b))+3*image->width*(j+(moy_yb/nb_b))]= 0;
				data[2+3*(i+(moy_xb/nb_b))+3*image->width*(j+(moy_yb/nb_b))]= 240;
			}
			if (nb_v > 0) {  
				xv = moy_xv/nb_v;
				yv = moy_yv/nb_v;
				data[0+3*(i+(moy_xv/nb_v))+3*image->width*(j+(moy_yv/nb_v))]= 240;
				data[1+3*(i+(moy_xv/nb_v))+3*image->width*(j+(moy_yv/nb_v))]= 0;
				data[2+3*(i+(moy_xv/nb_v))+3*image->width*(j+(moy_yv/nb_v))]= 240;
			}
			if (nb_r > 0) {  
				xr = moy_xr/nb_r;
				yr = moy_yr/nb_r;
				data[0+3*(i+(moy_xr/nb_r))+3*image->width*(j+(moy_yr/nb_r))]= 240;
				data[1+3*(i+(moy_xr/nb_r))+3*image->width*(j+(moy_yr/nb_r))]= 0;
				data[2+3*(i+(moy_xr/nb_r))+3*image->width*(j+(moy_yr/nb_r))]= 240;
			}
		}
	}
}



int traitement(){
	char key;
	IplImage *image;
	CvCapture *capture = cvCreateCameraCapture( CV_CAP_ANY );  //capte image caméra
	if (!capture) {
		printf("Ouverture du flux vidéo impossible !\n");
	}else{
		cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE); //fait la fenêtre

		while(key != 'q' && key != 'Q') {
			image  = cvQueryFrame(capture);
			//filtre_carre(image);
			filtre_forme(image);
			//affiche et attend entré clavier pendant 10ms
			cvShowImage("GeckoGeek Window", image);
			key = cvWaitKey(10);
		}
		//free tout
		cvDestroyAllWindows();
		cvReleaseCapture(&capture);
		printf("Merci d'avoir utilisé notre logiciel de capture d'image.\n");
	}
	return 0;
}

