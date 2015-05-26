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
GtkWidget* red_c = NULL;
IplImage *image_cam = NULL;
int learning = 0;
int xa = 20;
int init = 1;
int red_init = 0;
int green_init = 0;
int blue_init = 0;
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
gboolean expose_event_callback(GtkWidget *widget, GdkEventExpose *event, CvCapture * cap) //Requette Webcam + traitement de l'image
{
   if(init){

      gtk_widget_queue_draw( GTK_WIDGET( widget ));
      image_cam= traitement(cap); //cvQueryFrame(cap);
//	  for_gtk(image_cam);
      //filtre_forme(image_cam);
      image = convertOpenCv2Gtk(image_cam);


    gdk_draw_pixbuf( widget->window,
                    widget->style->fg_gc[ GTK_WIDGET_STATE( widget )],
                    convertOpenCv2Gtkp(image_cam),
                    0, 0, 0, 0,
                    image_cam->width,
                    image_cam->height,
                    GDK_RGB_DITHER_MAX,
                    0, 0);
    
//    red_up(red);
    }
 /*   else{
       printf("e");
        fflush(stdout);
        capture = cvCreateCameraCapture(CV_CAP_ANY);
        return FALSE;
    }*/
    return TRUE;

}
gboolean learning_mode(GtkWidget *widget, GdkEventExpose *event, GtkLabel *label) {
        
        if (learning){

                GtkWidget *name;
                gtk_label_set_text(label,"Mode Apprentissage");
                learning = 0;

                GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
                GtkWidget *button = gtk_button_new_from_stock (GTK_STOCK_ADD);
                gchar *title;
                
                gtk_container_set_border_width (GTK_CONTAINER (window), 25);
                name = gtk_label_new("Name");
                gtk_label_set_use_markup(GTK_LABEL(name), TRUE);
                gtk_container_add( GTK_CONTAINER (window), name);
                

                //gtk_container_add (GTK_CONTAINER (window), button);

                title = g_strdup_printf ("Window");
                gtk_window_set_title (GTK_WINDOW (window), title);
                g_free (title);
                        
                gtk_widget_show_all (window);
        }else{
                gtk_label_set_text(label,"");
                learning = 1;
        }       
        return TRUE;
}
gboolean init_red(GtkWidget *widget, GdkEventExpose *event, CvCapture *captur) {
       red_init = 1;
       green_init = blue_init = 0;
        gtk_label_set_text( GTK_LABEL(green),"Initialisation du rouge");
       // traitement();
        return TRUE;
}
gboolean init_green(GtkWidget *widget, GdkEventExpose *event, CvCapture *captur) {
       green_init = 1;
       red_init = blue_init = 0;
        gtk_label_set_text( GTK_LABEL(green),"Initialisation du vert");
       // traitement();
        return TRUE;
}
gboolean init_blue(GtkWidget *widget, GdkEventExpose *event, CvCapture *captur) {
       blue_init = 1;
       red_init = green_init = 0;
        gtk_label_set_text( GTK_LABEL(green),"Initialisation du bleu");
       // traitement();
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

static gboolean
button_press_callback (GtkWidget      *event_box,
                       GdkEventButton *event,
                       gpointer        data)
{
    g_print ("Event box clicked at coordinates %f,%f\n",
         event->x, event->y);
    int x = (int)event->x;
    int y = (int)event->y;
    // Returning TRUE means we handled the event, so the signal
    // emission should be stopped (don’t call any further callbacks
    // that may be connected). Return FALSE to continue invoking callbacks.
        CvScalar pixel;
        IplImage *hsv;
                hsv = RGBtoHSV(image_cam);
                pixel = cvGet2D(hsv, y, x);
                
                // Mets à jour la couleur rechercher
                
                if (red_init)
                    setHSV((int)pixel.val[0],(int)pixel.val[1],(int)pixel.val[2],0);
                 if (green_init)
                    setHSV((int)pixel.val[0],(int)pixel.val[1],(int)pixel.val[2],1);
                 if (blue_init)
                    setHSV((int)pixel.val[0],(int)pixel.val[1],(int)pixel.val[2],2);
 
                cvReleaseImage(&hsv);
    red_init = green_init = blue_init = 0;
    return TRUE;
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
    gtk_window_set_default_size(GTK_WINDOW(MainWindow), 800, 650); 
    layout = gtk_layout_new(NULL, NULL);//emplacement de la caméra
    gtk_container_add(GTK_CONTAINER (MainWindow), layout);
    gtk_widget_show(layout); 

    //*****show coord
      //Convertion du texte avec les balises
    red =gtk_label_new("Résultat");
    gtk_label_set_use_markup(GTK_LABEL(red), TRUE);
    gtk_layout_put(GTK_LAYOUT(layout), red, 300,600);

     GtkWidget *fixed;

    fixed = gtk_fixed_new();
    
    GtkWidget *event_box;

    event_box = gtk_event_box_new ();    

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request( drawing_area,640,480);
    gtk_container_add(GTK_CONTAINER(layout), fixed);
    //gtk_container_add( GTK_CONTAINER( layout ), drawing_area );
    gtk_container_add (GTK_CONTAINER (event_box), drawing_area);
    gtk_fixed_put(GTK_FIXED(fixed), event_box, 30, 100);
    g_signal_connect_swapped(G_OBJECT(MainWindow), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);//bouton quitter propre
    g_signal_connect( G_OBJECT( drawing_area), "expose_event", G_CALLBACK (expose_event_callback),capture);
    g_timeout_add(10, ( GSourceFunc )time_handler, ( gpointer )drawing_area );

//    GtkWidget *fixed;

    g_signal_connect (G_OBJECT (event_box),
                  "button_press_event",
                  G_CALLBACK (button_press_callback),
                  drawing_area);

  //  fixed = gtk_fixed_new();
    //gtk_container_add(GTK_CONTAINER(layout), fixed);

        
    green =gtk_label_new("Mode APPRENTISSAGE");
    gtk_label_set_use_markup(GTK_LABEL(green), TRUE);
    gtk_layout_put(GTK_LAYOUT(layout), green, 280,625);

    button = gtk_button_new_with_label("APPRENDRE");
    gtk_fixed_put(GTK_FIXED(fixed), button, 30, 50);
    gtk_widget_set_size_request(button, 130, 35);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(learning_mode), green);

    button = gtk_button_new_with_label("Rouge");
    gtk_fixed_put(GTK_FIXED(fixed), button, 680, 250);
    gtk_widget_set_size_request(button, 80, 35);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(init_red),capture);    

    button = gtk_button_new_with_label("Vert");
    gtk_fixed_put(GTK_FIXED(fixed), button, 680, 290);
    gtk_widget_set_size_request(button, 80, 35);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(init_green),capture);

    button = gtk_button_new_with_label("Bleu");
    gtk_fixed_put(GTK_FIXED(fixed), button, 680, 330);
    gtk_widget_set_size_request(button, 80, 35);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(init_blue),capture);


    //**********Start Programm *******
    gtk_widget_show_all(MainWindow);
    gtk_main();//begin infinite loop

   // g_source_remove (func_ref);//debrachement de la fonction dans la loop

    return 0;

}
