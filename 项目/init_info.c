#include "project.h"

int init_info()
{
    shut_down = false;
    mmap_p = mmap_creat();
    pthread_create(&pthread_touch_id , NULL , get_touch , NULL); 
    head = get_avi_name();
}