#ifndef GET_TOUCH_H
#define GET_TOUCH_H

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int touch_x;
int touch_y;
int pressure;


void * get_touch(void * arg);

#endif
