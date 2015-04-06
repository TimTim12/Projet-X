#include <stdio.h>
#include "highgui.h"
#include "cv.h"
#include <iostream>
#include <gtk/gtk.h>
#include "struct.h"


GtkWidget* convertOpenCv2Gtk (IplImage* srcImage);

gboolean callback(gpointer data);

int init_gtk(int argc, char **argv);
