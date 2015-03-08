#include "lib_gtk.h"
#include "traitement.h"

CvCapture *capture = NULL;
GtkWidget *layout = NULL;
GtkWidget *image = NULL;
GtkWidget * MainWindow = NULL;


GtkWidget* convertOpenCv2Gtk (IplImage* srcImage)
{
    GtkWidget* gtkImg = NULL;
    GdkPixbuf* gtkPixbuf = NULL;
    IplImage* dstImage = NULL;

    /** Creating the destionation image */
    dstImage = cvCreateImage( cvSize(srcImage->width,srcImage->height), IPL_DEPTH_8U, 3);

    /** Converting the format of the picture from BGR to RGB */
    cvCvtColor ( srcImage, dstImage, CV_BGR2RGB );

    /** Creates a new GdkPixbuf out of in-memory image data */
    gtkPixbuf = gdk_pixbuf_new_from_data ( (guchar*)dstImage->imageData,
            GDK_COLORSPACE_RGB,
            FALSE,
            dstImage->depth,
            dstImage->width,
            dstImage->height,
            (dstImage->widthStep),
            NULL,
            NULL
            );

    /** Create new GtkImage displaying pixbuf */
    gtkImg = gtk_image_new_from_pixbuf ( gtkPixbuf );
    return gtkImg;
}
gboolean callback(gpointer data) //Requette Webcam + traitement de l'image
{
      IplImage *image_cam= cvQueryFrame(capture);
      filtre_forme(image_cam);
      image = convertOpenCv2Gtk(image_cam);
      gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
      gtk_widget_queue_draw(layout);
      gtk_widget_show_all(MainWindow);

      return TRUE;

}
int init_gtk(int argc, char **argv){
    //***********Initialisation*************
    IplImage *image_cam;
    GtkWidget *button;

    capture = cvCreateCameraCapture(CV_CAP_ANY);//Initialisation de la camera
    image_cam = cvQueryFrame(capture);//requette pour une image

    gtk_init(&argc, &argv);//Initialisation de GTK
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(MainWindow), "Projet X");
    layout = gtk_layout_new(NULL, NULL);//emplacement de la caméra
    gtk_container_add(GTK_CONTAINER (MainWindow), layout);
   // gtk_widget_show(layout); 
    image = convertOpenCv2Gtk(image_cam);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    gint func_ref = g_timeout_add(100,callback,layout);//loop traitement image + affichage (callback)
    g_signal_connect_swapped(G_OBJECT(MainWindow), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);//bouton quitter propre

    //**********Start Programm *******
    gtk_widget_show_all(MainWindow);
    gtk_main();//begin infinite loop

    g_source_remove (func_ref);//debrachement de la fonction dans la loop

    return 0;

}
