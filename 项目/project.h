#ifndef _PROJECT__H_
#define _PROJECT__H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "show_bmp.h"
#include <stdlib.h>
#include <string.h>
#include "get_touch.h"
#include "myhead.h"
#include <dirent.h>

#define BRACKGROUND_BMP "./bmp/brackground.bmp"
#define AVI_ICON "./bmp/icon.bmp"
#define MAX_ICON 6
#define MEDIA_BUF "mplayer -quiet -slave -zoom -x 800 -y 380  -input file=/tmp/pipe ./avi/"//如果需要修改打开视频时候的参数，改这里就行
#define MEDIA_CONTROL_PIC "./bmp/media_control.bmp"
#define FIFO_PATH "/tmp/pipe"

unsigned int * combine_pixel(unsigned int * mmap_p , char * bmp_name);//刷图片
unsigned int * mmap_creat();//映射内存空间

unsigned int * mmap_p;//映射以后的指针
int touch_fd;//触摸屏的fd
bool shut_down;//关机标志
int pages;

struct avi_name_list//视频名字链表
{
    char name[30];
    struct avi_name_list * next;
    struct avi_name_list * prev;
};
int avi_num;
int icon_num;//记录总共要输出的图标数

struct avi_name_list * head;//视频名字链表头节点

struct avi_name_list * get_avi_name();

pthread_t pthread_touch_id;

void control_surface();//选择视频界面
char * get_name(int num);//根据第几个图标获得avi的名字
char * combine_chara(int num);//根据第几个图标的名字把视频播放的参数与avi名字合成
char * combine_bmp(int num);
int control_media(char * avi_name_buf);//视频控制台
int init_info();//初始化信息
void show_howmany_icon();
#endif
