#include<stdio.h>
#include<linux/input.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int posY;
int posX;

void set_coord_mouse(int fd, int x, int y);

int connect_mouse(char * file){
   int fd = open(file, O_RDWR);
   if(fd<1){
        printf("Error: Please check your input");
        return -1;
    }
   posX = 0;
   posY = 0;
   set_coord_mouse(fd,-10000,-10000);
   return fd;
}
void add_set_mouse(int fd, int x, int y){
    
    struct input_event eventx,eventy, event_end;
    memset(&eventx, 0, sizeof(eventx));//Reserve memory for event_x
    memset(&eventy, 0, sizeof(eventy));//Reserve memory for event_y
    memset(&event_end, 0, sizeof(event_end));//Reserve memory for event_end
    eventx.type = EV_REL;//set the type of the eventx to relative.
    eventx.code = REL_X;//tell the code of eventx to relative value of x.     
    eventx.value = x;
    write(fd, &eventx, sizeof(eventx));
    eventy.type = EV_REL;//set the type of the eventx to relative.
    eventy.code = REL_Y;//tell the code of eventx to relative value of y.     
    eventy.value = y;
    write(fd, &eventy, sizeof(eventy));
    event_end.type = EV_SYN;
    event_end.code = SYN_REPORT;
    event_end.value = 0;
    write(fd, &event_end, sizeof(event_end));


}
void set_coord_mouse(int fd, int x, int y){
    int newX,newY = 0;
    newX = x - posX;
    newY = y - posY;
    struct input_event eventx,eventy, event_end;
    memset(&eventx, 0, sizeof(eventx));//Reserve memory for event_x
    memset(&eventy, 0, sizeof(eventy));//Reserve memory for event_y
    memset(&event_end, 0, sizeof(event_end));//Reserve memory for event_end
      
      
    eventx.type = EV_REL;//set the type of the eventx to relative.
    eventx.code = REL_X;//tell the code of eventx to relative value of x. 

    eventx.value = newX;
    write(fd, &eventx, sizeof(eventx));
    eventy.type = EV_REL;//set the type of the eventx to relative.
    eventy.code = REL_Y;//tell the code of eventx to relative value of y.     

    printf("add: %d \n",x - posX);
    eventy.value = newY;
    write(fd, &eventy, sizeof(eventy));
    event_end.type = EV_SYN;
    event_end.code = SYN_REPORT;
    event_end.value = 0;
    write(fd, &event_end, sizeof(event_end));

    if(x != -10000){ //avoid initialisation
    posX = x;
    posY = y;
    }
}/*
int main()
{
    
    int fd = connect_mouse("/dev/input/event12");
    printf("Connection souris OK!!\n");
    int x = 50;
    printf("entrez une coord en X? \n");
    //scanf("%d \n",&x);
    printf("déplacement de %d \n", x);
               set_coord_mouse(fd,x,0);
    printf("fin de déplacement \n");
               // scanf("%d \n",&x);

    close(fd);
    return 0;
}*/
