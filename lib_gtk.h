#include <stdio.h>
#include "highgui.h"
#include "cv.h"
#include <iostream>
#include <gtk/gtk.h>
#include "struct.h"

char* getFigureName();
void setFigureName(char* name);
void setLearning(int i);
int getLearning();
void setRecording(int i);
int getRecording();
GtkWidget* convertOpenCv2Gtk (IplImage* srcImage);
void update_reco_label();

gboolean callback(gpointer data);

int init_gtk(int argc, char **argv);
