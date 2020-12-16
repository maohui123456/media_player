#include "project.h"
//这个函数用来合成像素点的
//把bmp图片的三个像素点和成lcd的四个像素点，并返回合成以后的内存地址
//合成的像素是从上到下，从左到右存放在内存中
unsigned int * combine_pixel(unsigned int * mmap_p , char * bmp_name)
{
    int i,j;
    //打开bmp文件
    FILE * bmp_fp = fopen(bmp_name , "r");
    if(bmp_fp == NULL)
    {
        //perror("打开bmp失败\n");
        return NULL;
    }

    //定义bmp文件头结构体指针
    struct bitmap_header * header_info = calloc(1 , 14);
    struct bitmap_info * bmp_info = calloc(1 , 40);
    bzero(header_info , 14);
    bzero(bmp_info , 40);

    //把头54把头54个字节读取放在上面的内存里
    //顺便跳过文件头54个字节
    fread(header_info , 1 , 14 , bmp_fp);
    fread(bmp_info , 1 , 40 , bmp_fp);

    //获得bmp文件的宽和高
    bmp_width = bmp_info->width;
    bmp_height = bmp_info->height;
    //对于像素点不是4的倍数的点每一行需要对bmp像素补的点的字节数
    int ret = (4-(bmp_width)*3%4)%4;

    //测试用
    // printf("bmp_width = %d\n" , bmp_width);
    // printf("bmp_height = %d\n" , bmp_height);
    // printf("ret = %d\n" , ret);
    // printf("%d" , bmp_width*3+ret);

    //创建存储lcd像素点的指针
    //创建存储bmp像素点的指针
                                           //每一行需要补的点记得乘上行数
    unsigned char * bmp_pixel = calloc(1 , bmp_width*bmp_height*3+ret*bmp_height);
    unsigned int * lcd_pixel = calloc(1 , bmp_width*bmp_height*4);
    bzero(bmp_pixel , bmp_width*bmp_height*3+ret*bmp_height);
    bzero(lcd_pixel , bmp_width*bmp_height*4);

    //把bmp像素点读都bmp——pixel中
    fread(bmp_pixel , bmp_width*bmp_height*3 + ret*bmp_height , 1 , bmp_fp);

    fclose(bmp_fp);

    //刷黑屏的测试
    // for(i = 0 ; i < 480 ; i++)
    // {
    //     for(j = 0 ; j < 800 ; j++)
    //     {
    //         mmap_p[800*i + j ] = 0x000000;
    //     }
    // }

    //把bmp像素点合成为lcd像素点
    for(i = 0 ; i < bmp_height ; i++)
    {
        for(j = 0 ; j < bmp_width ; j++)
        {
            //对于倍数不是4的倍数合成像素点的时候记得每合成一行就加上补的像素点，就是下面的ret*i
            lcd_pixel[bmp_width*i + j] = (bmp_pixel[3*bmp_width*i + j*3 +ret*i]<<0) | (bmp_pixel[3*bmp_width*i + j*3 +1 +ret*i]<<8) | (bmp_pixel[3*bmp_width*i + j*3 +2 +ret*i]<<16);
        }
    }
    //返回合成好的像素点
    return lcd_pixel;
}
