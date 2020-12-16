#include "project.h"
//这个函数用来显示bmp图片，把合成好的像素点输出出来
//并且图片输出的位置根据用户的需要
int show_bmp_according_x_y (unsigned int * mmap_p , char * bmp_name , int x , int y , int flame_w , int flame_h)
{
    int i , j;
    unsigned int * lcd_pixel_p = calloc(1 , bmp_width*bmp_height*4);
    bzero(lcd_pixel_p , bmp_width*bmp_height*4);

    lcd_pixel_p = combine_pixel(mmap_p , bmp_name);
    if(lcd_pixel_p == NULL)
    {
        return -1;
    }

    ordinary_x = x ;
    ordinary_y = y ;
    
    //判断像素点是否超过边框
    if(bmp_width > 800)
    {
        ordinary_x = 0;
    }
    if(bmp_height > 480)
    {
        ordinary_y = 0;
    }
    
    //把数据传入的映射的指针里面
    for(i = 0 ; i < bmp_height && i<flame_h; i++)
    {
        for(j = 0 ; j < bmp_width && j<flame_w; j++)
        {
            mmap_p[800*i + j + ordinary_x + ordinary_y*800] = lcd_pixel_p[(bmp_height-1-i)*bmp_width + j];
        }
    }
    // printf("ordinary_x = %d\n" , ordinary_x);
    // printf("ordinary_y = %d\n" , ordinary_y);
    return 0 ;
}
