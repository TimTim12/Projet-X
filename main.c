#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>


/*IplImage filtre(IplImage *img){
	int i, j;
	unsigned char* data = (unsigned char*)(img->imageData);
	for (i = 0; i < img->height; i++){
		for (j = 0; j < img->width;j++){
			if (data[2+3*j+3*img->width*i] > 200){
				data[0+3*j+3*img->width*i] = 0; //bleu
				data[1+3*j+3*img->width*i] = 0; //vert
				data[2+3*j+3*img->width*i] = 255; //rouge
			}
		}
	}
	return img;

}*/


int main (int argc, char* argv[]){
	char key;
	int i = 0, j = 0;
	IplImage *image;
	CvCapture *capture = cvCreateCameraCapture( CV_CAP_ANY );
	if (!capture) {
   		printf("Ouverture du flux vidéo impossible !\n");
	}else{
		cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE);

		while(key != 'q' && key != 'Q') {
			image  = cvQueryFrame(capture);
			unsigned char* data = (unsigned char*)(image->imageData);
			for (i = 0; i < image->height; i++){
				for (j = 0; j < image->width;j++){
					if (data[2+3*j+3*image->width*i] > 200 && data[0+3*j+3*image->width*i] < 200 && data[1+3*j+3*image->width*i] < 200){
						data[0+3*j+3*image->width*i] = 0; //bleu
						data[1+3*j+3*image->width*i] = 0; //vert
						data[2+3*j+3*image->width*i] = 255; //rouge
					}
				}
			}
			cvShowImage("GeckoGeek Window", image);
			key = cvWaitKey(10);
		}
		cvDestroyAllWindows();
		cvReleaseCapture(&capture);
		printf("Merci d'avoir utilisé notre logiciel de capture d'image.\n");
	}
	return 0;
}
