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
	int i = 0, j = 0, h = 0, w = 0;
	IplImage *image;
	CvCapture *capture = cvCreateCameraCapture( CV_CAP_ANY );  //capte image caméra
	if (!capture) {
   		printf("Ouverture du flux vidéo impossible !\n");
	}else{
		cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE); //fait la fenêtre

		while(key != 'q' && key != 'Q') {
			image  = cvQueryFrame(capture);

		//fonction à mettre ailleurs que dans main
		//{
			unsigned char* data = (unsigned char*)(image->imageData);
			for (i = 0; i < image->height; i++){   //set les pixels
				for (j = 0; j < image->width;j++){
					if (data[2+3*j+3*image->width*i] > 200 && data[0+3*j+3*image->width*i] < 100 && data[1+3*j+3*image->width*i] < 100){
						data[0+3*j+3*image->width*i] = 0; //bleu
						data[1+3*j+3*image->width*i] = 0; //vert
						data[2+3*j+3*image->width*i] = 255; //rouge
						w = j - 40;
						if (w > 0 && data[2+3*w+3*image->width*i] != 255 ){
							for(h = i - 20; h < i + 20; h++){
								if (h > 0 && h < image->height ){
								data[0+3*w+3*image->width*h] = 0; 
								data[1+3*w+3*image->width*h] = 255; 
								data[2+3*w+3*image->width*h] = 0; 
								}
							}
						}
						w = j + 40;
						if (w < image->width && data[2+3*w+3*image->width*i] != 255 ){
							for(h = i - 20; h < i + 20; h++){
								if (h > 0 && h < image->height ){
								data[0+3*w+3*image->width*h] = 0;
								data[1+3*w+3*image->width*h] = 255; 
								data[2+3*w+3*image->width*h] = 0;
								}
							}
						}
						h = i - 40;
						if (h > 0 && data[2+3*j+3*image->width*h] != 255 ){
							for(w = j - 20; w < j + 20; w++){
								if (w > 0 && w < image->height ){
								data[0+3*w+3*image->width*h] = 0; 
								data[1+3*w+3*image->width*h] = 255; 
								data[2+3*w+3*image->width*h] = 0; 
								}
							}
						}
						h = i + 40;
						if (h < image->height && data[2+3*j+3*image->width*h] != 255 ){
							for(w = j - 20; w < j + 20; w++){
								if (w > 0 && w < image->height ){
								data[0+3*w+3*image->width*h] = 0; 
								data[1+3*w+3*image->width*h] = 255; 
								data[2+3*w+3*image->width*h] = 0;
								}
							}
						}
					}
				}
			}
		//}

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
