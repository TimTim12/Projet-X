#include "lib_gtk.h"
#include "traitement.h"
#include <string.h>

CvCapture *capture = NULL;
GtkWidget *layout = NULL;
GtkWidget *image = NULL;
GtkWidget * MainWindow = NULL;
GtkWidget* red = NULL;
GtkWidget* green = NULL;
GtkWidget* blue = NULL;
GtkWidget* red_c = NULL;
GtkWidget* green_c = NULL;
GtkWidget* blue_c = NULL;
int xa = 50;

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
GdkPixbuf* convertOpenCv2Gtkp (IplImage* srcImage)
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
  //  gtkImg = gtk_image_new_from_pixbuf ( gtkPixbuf );
    return gtkPixbuf;
}
gboolean callback(gpointer data) //Requette Webcam + traitement de l'image
{
   // xa = xa * -1; 
      IplImage *image_cam= cvQueryFrame(capture);
      filtre_forme(image_cam);
      image = convertOpenCv2Gtk(image_cam);
      gtk_layout_put(GTK_LAYOUT(data), image, 0, 0);
      gtk_widget_queue_draw(GTK_WIDGET(data));
   //   gtk_widget_show_all(MainWindow);
   //   g_usleep(150);
   
    gtk_widget_show_all(GTK_WIDGET(data));
    return TRUE;

}
gboolean red_up( gpointer label)
{
    int x = get_xr();
    int y = get_yr();
    char *new_coord = g_strdup_printf("%dx%d",x,y);
    gtk_label_set_text(GTK_LABEL(label), new_coord);
}
gboolean green_up( gpointer label)
{
    int x = get_xv();
    int y = get_yv();
    char *new_coord = g_strdup_printf("%dx%d",x,y);
    gtk_label_set_text(GTK_LABEL(label),new_coord);
}
gboolean blue_up( gpointer label)
{
    int x = get_xb();
    int y = get_yb();
    char *new_coord = g_strdup_printf("%dx%d",x,y);
    gtk_label_set_text(GTK_LABEL(label),new_coord);
}
int init_gtk(int argc, char **argv){
    //***********Initialisation*************
    IplImage *image_cam;
    GtkWidget *button;
    gchar* TexteConverti = NULL;

    capture = cvCreateCameraCapture(CV_CAP_ANY);//Initialisation de la camera
    image_cam = cvQueryFrame(capture);//requette pour une image

    gtk_init(&argc, &argv);//Initialisation de GTK
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(MainWindow), "Projet X");
    gtk_window_set_default_size(GTK_WINDOW(MainWindow), 400, 400); 
    layout = gtk_layout_new(NULL, NULL);//emplacement de la caméra
    gtk_container_add(GTK_CONTAINER (MainWindow), layout);
    gtk_widget_show(layout); 
    image = convertOpenCv2Gtk(image_cam);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    //*****show coord
      //Convertion du texte avec les balises
    red =gtk_label_new("Rouge:");
    blue =gtk_label_new("Bleu:");
    green =gtk_label_new("Vert:");
    red_c =gtk_label_new("");
    blue_c =gtk_label_new("");
    green_c =gtk_label_new("");


    gtk_label_set_use_markup(GTK_LABEL(red), TRUE);
    gtk_label_set_use_markup(GTK_LABEL(blue), TRUE);
    gtk_label_set_use_markup(GTK_LABEL(green), TRUE);
    gtk_layout_put(GTK_LAYOUT(layout), red, 20,600);
    gtk_layout_put(GTK_LAYOUT(layout), green, 220,600);
    gtk_layout_put(GTK_LAYOUT(layout), blue, 420,600);
    gtk_layout_put(GTK_LAYOUT(layout), red_c, 70,600);
    gtk_layout_put(GTK_LAYOUT(layout), green_c, 260,600);
    gtk_layout_put(GTK_LAYOUT(layout), blue_c, 460,600);

    int func_ref = g_timeout_add_full(G_PRIORITY_HIGH,130,callback,layout ,NULL);//loop traitement image + affichage (callback)
    g_signal_connect_swapped(G_OBJECT(MainWindow), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);//bouton quitter propre
    g_timeout_add_full(G_PRIORITY_HIGH,100,red_up,red_c,NULL);
    g_timeout_add_full(G_PRIORITY_HIGH,100,green_up,green_c,NULL);
    g_timeout_add_full(G_PRIORITY_HIGH,100,blue_up,blue_c ,NULL);
  //  g_timeout_add_full(G_PRIORITY_HIGH,100,green_up,green ,NULL);
  //  g_timeout_add_full(G_PRIORITY_HIGH,100,blue_up,blue ,NULL);

    //**********Start Programm *******
    gtk_widget_show_all(MainWindow);
    gtk_main();//begin infinite loop

    g_source_remove (func_ref);//debrachement de la fonction dans la loop

    return 0;

}
