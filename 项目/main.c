#include "project.h"

int main(char argc ,const char ** argv)
{
    init_info();
    control_surface();
    pthread_join(pthread_touch_id , NULL);
    //while(1);
    return 0;
    //test vi 
}
