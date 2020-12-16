#include "project.h"
#define display_path "/dev/fb0" //lcd屏幕的路径
#define display_size 800*480*4 //像素点占的字节大小
unsigned int * mmap_creat()
{
    //打开lcd文件
    int lcd_fd = open(display_path , O_RDWR);
    if(lcd_fd == -1)
    {
        perror("mmap falied !\n");
        return NULL;
    }
    //映射lcd的像素
    unsigned int * mmap_p_creat = mmap(NULL , display_size ,PROT_READ | PROT_WRITE , MAP_SHARED , lcd_fd , 0);
    if(mmap_p_creat == MAP_FAILED)
    {
        printf("映射失败\n");
        return NULL;
    }
    close(lcd_fd);
    return mmap_p_creat;
}
