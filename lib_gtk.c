#include "lib_gtk.h"
#include <gtk/gtk.h>
#include "traitement.h"
#include "struct.h"
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
int xa = 20;

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
    return gtkPixbuf;
}

gboolean red_up( gpointer label)
{
    int x = get_xr();
    int y = get_yr();
    char *new_coord = g_strdup_printf("%dx%d",x,y);
    gtk_label_set_text(GTK_LABEL(label), new_coord);
    return TRUE;
}
gboolean green_up( gpointer label)
{
    int x = get_xv();
    int y = get_yv();
    char *new_coord = g_strdup_printf("%dx%d",x,y);
    gtk_label_set_text(GTK_LABEL(label),new_coord);
    
    return TRUE;
}
gboolean blue_up( gpointer label)
{
    int x = get_xb();
    int y = get_yb();
    char *new_coord = g_strdup_printf("%dx%d",x,y);
    gtk_label_set_text(GTK_LABEL(label),new_coord);

        
    return TRUE;
}
static gboolean time_handler( GtkWidget *widget) {
      return TRUE;
}
gboolean expose_event_callback(GtkWidget *widget, GdkEventExpose *event, GtkWidget *red) //Requette Webcam + traitement de l'image
{ 
      gtk_widget_queue_draw( GTK_WIDGET( widget ));
      IplImage *image_cam= cvQueryFrame(capture);
	  for_gtk(image_cam);
      //filtre_forme(image_cam);
     // image = convertOpenCv2Gtk(image_cam);


    gdk_draw_pixbuf( widget->window,
                    widget->style->fg_gc[ GTK_WIDGET_STATE( widget )],
                    convertOpenCv2Gtkp(image_cam),
                    0, 0, 0, 0,
                    image_cam->width,
                    image_cam->height,
                    GDK_RGB_DITHER_MAX,
                    0, 0);
    
 //   red_up(red);

    return TRUE;

}
void OnScrollbarChange(GtkWidget *pWidget, gpointer data)
{
   gchar* sLabel;
   gint iValue;

   /* Récupération de la valeur de la scrollbar */
   iValue = gtk_range_get_value(GTK_RANGE(pWidget));
   /* Création du nouveau label */
   sLabel = g_strdup_printf("%d", iValue);
   /* Modification du label */
   gtk_label_set_text(GTK_LABEL(data), sLabel);
   /* Liberation memoire */
   g_free(sLabel);
}

int init_gtk(int argc, char **argv){
    //***********Initialisation*************
    IplImage *image_cam;
    GtkWidget *button;
    gchar* TexteConverti = NULL;
    GtkWidget *drawing_area;
    capture = cvCreateCameraCapture(CV_CAP_ANY);//Initialisation de la camera
    image_cam = cvQueryFrame(capture);//requette pour une image

    gtk_init(&argc, &argv);//Initialisation de GTK
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(MainWindow), "Projet X");
    gtk_window_set_default_size(GTK_WINDOW(MainWindow), 600, 600); 
    layout = gtk_layout_new(NULL, NULL);//emplacement de la caméra
    gtk_container_add(GTK_CONTAINER (MainWindow), layout);
    gtk_widget_show(layout); 
    //image = convertOpenCv2Gtk(image_cam);
    //gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    //*****show coord
      //Convertion du texte avec les balises
    red =gtk_label_new("Rouge:");
    blue =gtk_label_new("Bleu:");
    green =gtk_label_new("Vert:");
    red_c =gtk_label_new("0x0");
   /* blue_c =gtk_label_new("");
    green_c =gtk_label_new("");

*/
    gtk_label_set_use_markup(GTK_LABEL(red), TRUE);/*
    gtk_label_set_use_markup(GTK_LABEL(blue), TRUE);
    gtk_label_set_use_markup(GTK_LABEL(green), TRUE);*/
    gtk_layout_put(GTK_LAYOUT(layout), red, 20,600);
  //  gtk_layout_put(GTK_LAYOUT(layout), green, 220,600);
  //  gtk_layout_put(GTK_LAYOUT(layout), blue, 420,600);
    gtk_layout_put(GTK_LAYOUT(layout), red_c, 70,600);
  //  gtk_layout_put(GTK_LAYOUT(layout), green_c, 260,600);
  //  gtk_layout_put(GTK_LAYOUT(layout), blue_c, 460,600);

/*   GtkWidget *pLabel;
   GtkWidget *pScrollbar;
   GtkObject *Adjust;
   GtkWidget *pColorBox;
   GtkWidget *pFrame;
   GtkWidget *pMainVBox;

   pMainVBox = gtk_vbox_new(TRUE, 0);
   gtk_layout_put(GTK_LAYOUT(layout), pMainVBox,500,500);

   pFrame = gtk_frame_new("Rouge");
   gtk_box_pack_start(GTK_BOX(pMainVBox), pFrame, FALSE, FALSE, 0);
   pColorBox = gtk_vbox_new(TRUE, 0);
   gtk_container_add(GTK_CONTAINER(pFrame), pColorBox);*/

   /* Label d'affichage de valeur R*/
 //  pLabel = gtk_label_new("0");
 //  gtk_box_pack_start(GTK_BOX(pColorBox), pLabel, FALSE, FALSE, 0);
   /* Création d un GtkAdjustment */
 //  Adjust = gtk_adjustment_new(0, 0, 256, 1, 10, 1);
   /* Creation d une scrollbar horizontale*/
 //  pScrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(Adjust));
 //  gtk_box_pack_start(GTK_BOX(pColorBox), pScrollbar, TRUE, TRUE, 0);
   /* Connexion du signal pour modification de l'affichage */
   //g_signal_connect(G_OBJECT(pScrollbar), "value-changed",
//      G_CALLBACK(OnScrollbarChange), (GtkWidget*)pLabel);

  //  int func_ref = g_timeout_add_full(G_PRIORITY_HIGH,130,callback,image ,NULL);//loop traitement image + affichage (callback)
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request( drawing_area,900,500);
    gtk_container_add( GTK_CONTAINER( layout ), drawing_area );
    g_signal_connect_swapped(G_OBJECT(MainWindow), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);//bouton quitter propre
//    g_timeout_add_full(G_PRIORITY_HIGH,200,red_up,red_c,NULL);
//  g_timeout_add_full(G_PRIORITY_HIGH,200,green_up,green_c,NULL);
//  g_timeout_add_full(G_PRIORITY_HIGH,200,blue_up,blue_c ,NULL);
  //  g_timeout_add_full(G_PRIORITY_HIGH,100,green_up,green ,NULL);
  //  g_timeout_add_full(G_PRIORITY_HIGH,100,blue_up,blue ,NULL);
    g_signal_connect( G_OBJECT( drawing_area), "expose_event", G_CALLBACK (expose_event_callback),red_c);
    g_timeout_add( 130, ( GSourceFunc )time_handler, ( gpointer )drawing_area );
 
    //**********Start Programm *******
    gtk_widget_show_all(MainWindow);
    gtk_main();//begin infinite loop

   // g_source_remove (func_ref);//debrachement de la fonction dans la loop

    return 0;

}
