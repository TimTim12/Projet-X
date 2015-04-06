#include<stdio.h>
#include<linux/input.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

void set_coord_mouse(int fd, int x, int y);

void add_set_mouse(int fd, int x, int y);

int connect_mouse(char * file);
