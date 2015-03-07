#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>

int main (int argc, char* argv[]){
	char key;
	IplImage *image;
	CvCapture *capture = cvCreateCameraCapture( CV_CAP_ANY );
	if (!capture) {
   		printf("Ouverture du flux vidéo impossible !\n");
	}else{
		cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE);

		while(key != 'q' && key != 'Q') {
			image  = cvQueryFrame(capture);
			cvShowImage("GeckoGeek Window", image);
			key = cvWaitKey(10);
		}
		cvDestroyAllWindows();
		cvReleaseCapture(&capture);
		printf("Merci d'avoir utilisé notre logiciel de capture d'image.\n");
	}
	return 0;
}
