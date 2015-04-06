#include <stdlib.h>
#include <stdio.h>
#include "traitement.h"
#include "cv.h"
#include <iostream>
#include <gtk/gtk.h>
#include "lib_gtk.h"
#include<string.h>
#include "struct.h"

int main(int argc,char* argv[]) {
    if(strcmp(argv[1],"opencv"))   
        init_gtk(argc, argv);
    else if(strcmp(argv[1],"gtk"))
        traitement();
    return 0;
}
