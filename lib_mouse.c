#include<stdio.h>
#include"cv.h"
#include "highgui.h"
#include<linux/input.h>
#include<fcntl.h>
#include<unistd.h>

int connect_mouse(char * file){
   int fd = open(file, O_RDWR);
   if(fd<1){
        printf("Error: Please check your input");
        return -1;
    }
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
    int posX,posY;
    int lastX,lastY;
    struct input_event eventx,eventy, event_end;
    memset(&eventx, 0, sizeof(eventx));//Reserve memory for event_x
    memset(&eventy, 0, sizeof(eventy));//Reserve memory for event_y
    memset(&event_end, 0, sizeof(event_end));//Reserve memory for event_end
    if( (read(fd, &eventx, sizeof(eventx))) == -1)
        printf("error");
    lastX = eventx.value;
    printf("lastx: %d \n",lastX);
    fflush(stdout);
    read(fd, &eventy, sizeof(eventy));
    lastY = eventy.value;
    memset(&eventx, 0, sizeof(eventx));//Reserve memory for event_x
    memset(&eventy, 0, sizeof(eventy));//Reserve memory for event_y
    eventx.type = EV_REL;//set the type of the eventx to relative.
    eventx.code = REL_X;//tell the code of eventx to relative value of x.     
    posX = x - lastX;
    eventx.value = posX;
    write(fd, &eventx, sizeof(eventx));
    eventy.type = EV_REL;//set the type of the eventx to relative.
    eventy.code = REL_Y;//tell the code of eventx to relative value of y.     
    posY = y - lastY;
    printf("%d \n",posY);
    eventy.value = posY;
    write(fd, &eventy, sizeof(eventy));
    event_end.type = EV_SYN;
    event_end.code = SYN_REPORT;
    event_end.value = 0;
    write(fd, &event_end, sizeof(event_end));


}
int main()
{
    
    int fd = connect_mouse("/dev/input/event12");
    int posX;
    while(1){
        scanf("%d \n",&posX);
        for(int i = 0;i<2;i++)
        set_coord_mouse(fd,posX,100);
    }
    close(fd);
    return 0;
}
