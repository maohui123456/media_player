#include "project.h"
#include "get_touch.h"
#define TS_MODE 1
void  * get_touch(void * arg)
{
    while(1)
    {
    //打开触摸屏文件
    int touch_fd = open("/dev/input/event0" , O_RDWR);
    if(touch_fd == -1)
    {
        perror("打开触摸屏文件失败\n");
        return NULL;
    }
    // 申请存放结构体信息的结构体变量
    struct input_event touch_buf;
    bzero(&touch_buf , sizeof(touch_buf));

    while(1)
    {
        //读取触摸屏文件的信息，存放到结构体变量里面
        read(touch_fd , &touch_buf , sizeof(touch_buf));
        //获取触摸屏坐标
        if(touch_buf.type == EV_ABS)
        {
            //获取x
            if(touch_buf.code == ABS_X )
            {

                touch_x = touch_buf.value;
#if  TS_MODE

				touch_x=touch_x*800/1024;
#endif
            }
            //获取y
            else if(touch_buf.code == ABS_Y)
            {
                touch_y = touch_buf.value;
#if  TS_MODE

				touch_y=touch_y*480/600;
#endif
            }
        }
        //判断手指点击之后是否离开屏幕
        else if(touch_buf.type == EV_KEY)
        {
            if(touch_buf.code == BTN_TOUCH)
            {
                pressure = touch_buf.value;
                if(pressure == 1)
                {

                    // printf("y=%d\n" , touch_y);
                    // printf("x=%d\n" , touch_x);
                    continue;
                }
                else if (pressure == 0)
                {

                    // printf("y=%d\n" , touch_y);
                    // printf("x=%d\n" , touch_x);
                    break;
                }
            }
        }

    }
    close(touch_fd);
    if(shut_down)
    {
        pthread_exit(NULL);
    }
    // return NULL;
    }
}
