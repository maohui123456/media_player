#include "project.h"
//这个函数用来显示bmp图片，把合成好的像素点输出出来
//并且图片在全屏模式下始终输出在中间
void show_bmp_middle (unsigned int * mmap_p , char * bmp_name , int flame_w , int flame_h)
{
    int i , j;
    unsigned int * lcd_pixel_p = calloc(1 , bmp_width*bmp_height*4);
    bzero(lcd_pixel_p , bmp_width*bmp_height*4);

    lcd_pixel_p = combine_pixel(mmap_p , bmp_name);

    ordinary_x = (800-bmp_width)/2 ;
    ordinary_y = (480-bmp_height)/2 ;
    if(bmp_width > 800)
    {
        ordinary_x = 0;
    }
    if(bmp_height > 800)
    {
        ordinary_y = 0;
    }
    //把数据传入的映射的指针里面
    for(i = 0 ; i < bmp_height && i<flame_h ; i++)
    {
        for(j = 0 ; j < bmp_width && j<flame_w ; j++)
        {
            mmap_p[800*i + j + ordinary_x + ordinary_y*800] = lcd_pixel_p[(bmp_height-1-i)*bmp_width + j];
        }
    }

    return ;
}
