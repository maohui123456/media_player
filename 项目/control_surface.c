#include "project.h"


int ret_val;
static int icon_num_now;//记录当前界面要输出的图标数
//当前页数

void show_howmany_icon()
{
    int icon_x = 100;
    int icon_y = 40;
    int ret;
    char * buf = malloc(512);
    bzero(buf , 512);
    if(icon_num <= 0)
    {
        printf("已经没有视频啦\n");
        icon_num_now = 0;
        return ;
    }
    icon_num -= MAX_ICON;
    if(icon_num >= 0)
    {
        for(int i = 0 ; i < MAX_ICON ; i++)
        {
            strcpy(buf , combine_bmp(i));
            //printf("%s\n" , combine_bmp(i));
            
            ret = show_bmp_according_x_y(mmap_p , buf , icon_x , icon_y , 800 , 480);
            if(ret == -1)
            {
                //printf("打开默认图标\n");
                show_bmp_according_x_y(mmap_p , AVI_ICON , icon_x , icon_y , 800 , 480);
            }
            

            if(i < 2)
            icon_x+=230;
            if(i == 2)
            {
                icon_y += 220;
                icon_x = 100;
            }
            else if( i >= 3)
            {
                icon_x+=100 + 130;
            }
        }
        icon_num_now = MAX_ICON;
    }
    else
    {
        for(int i = 0 ; i < icon_num + MAX_ICON ; i++)
        {
            ret = show_bmp_according_x_y(mmap_p , combine_bmp(i+(pages-1)*6) , icon_x , icon_y , 800 , 480);
            if(ret == -1)
            show_bmp_according_x_y(mmap_p , AVI_ICON , icon_x , icon_y , 800 , 480);
            
            //printf("%s\n" , combine_bmp(i+(pages-1)*6));

            if(i < 2)
            icon_x+=230;
            if(i == 2)
            {
                icon_y += 220;
                icon_x = 100;
            }
            else if( i >= 3)
            {
                icon_x+=100 + 130;
            }
        }
        icon_num_now = icon_num + MAX_ICON;
    }
}

char * get_name(int num)
{
    struct avi_name_list * p = head;//拿出名字链表头节点
    for(int i = 0 ; i < num ; i++)//找到对应序号的节点
    p = p->next;
    //printf("输出视频%s\n" , p->name);
    return p->name;
}

char * combine_chara(int num)
{
    char * buf = malloc(512);
    bzero(buf , 512);
    sprintf(buf , "%s%s &" , MEDIA_BUF ,get_name(num));
    return buf;
}

char * combine_bmp(int num)
{
    char * buf = malloc(512);
    char * new_buf = malloc(512);
    bzero(buf , 512);
    bzero(new_buf , 512);
    strcpy(buf , get_name(num+1));
    strtok(buf , ".");
    sprintf(new_buf , "./bmp/%s.bmp" , buf);
    return new_buf;
}

void media_play(int a)
{
    if(pages == 1)//在第一页的情况下
    {
        ret_val = control_media(combine_chara(a));
        while(ret_val != 0)
        {
            while(ret_val == 1)//判断是不是需要播放下一个视频
            {
                touch_x = 0;
                touch_y = 0;
                a++;
                if(a > avi_num)//如果没有视频就直接返回第一页主页面
                {
                    printf("没有视频啦\n");
                    show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480);
                    icon_num = avi_num;//从第一页开始
                    show_howmany_icon();
                    ret_val = 0; 
                    break;
                }
                ret_val = control_media(combine_chara(a));
            }
            while(ret_val == -1)//判断是不是需要播放上一个视频
            {
                touch_x = 0;
                touch_y = 0;
                a--;
                if(a == 0)//如果没有视频就直接返回第一页主页面
                {
                    printf("没有视频啦\n");
                    show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480);
                    icon_num = avi_num;//从第一页开始
                    show_howmany_icon();
                    ret_val = 0; 
                    break;
                }
                ret_val = control_media(combine_chara(a));
            }
        } 
    }
    else//在其他页的情况下
    {
        ret_val = control_media(combine_chara(a+(pages-1)*6));
        while(ret_val != 0)
        {
            while(ret_val == 1)//判断是不是需要播放上一个视频
            {
                touch_x = 0;
                touch_y = 0;
                a++;
                if(a+(pages-1)*6 > avi_num)//如果没有视频就直接返回第一页主页面
                {
                    printf("没有视频啦\n");
                    show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480);
                    icon_num = avi_num;//从第一页开始
                    show_howmany_icon();
                    ret_val = 0; 
                    break;
                }
                ret_val = control_media(combine_chara(a+(pages-1)*6));
            }

            while(ret_val == -1)//判断是不是需要播放下一个视频
            {
                touch_x = 0;
                touch_y = 0;
                a--;
                if(a+(pages-1)*6 == 0)//如果没有视频就直接返回第一页主页面
                {
                    printf("没有视频啦\n");
                    show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480);
                    icon_num = avi_num;//从第一页开始
                    show_howmany_icon();
                    ret_val = 0; 
                    break;
                }
                ret_val = control_media(combine_chara(a+(pages-1)*6));
            }
        }   
    }  
    ret_val = 0;  
    // show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480);
    // icon_num = avi_num;//从第一页开始
    // show_howmany_icon();
}

void control_surface()
{
    
    icon_num = avi_num;
    pages = 1;
    show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480);
    show_howmany_icon();
    while(1)
    {
        if(touch_x > 700 && touch_x < 800 && touch_y > 360 && touch_y <480 && pressure == 0)//下一页
        {
            int max_pages;
            if(avi_num%6 != 0)
            {
               max_pages = avi_num/6 + 1;
            }
            else
            {
                max_pages = avi_num/6;
            }
            if(pages >= max_pages)
            continue;
            pages++;
            touch_x = 0;
            touch_y = 0;
            show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480 );
            show_howmany_icon();
            
        }
        
        if(touch_x > 0 && touch_x < 100 && touch_y > 360 && touch_y <480 && pressure == 0)//上一页
        {
            if(pages == 1)
            continue;
            icon_num = avi_num;
            touch_x = 0;
            touch_y = 0;
            show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480);
            show_howmany_icon();
            pages--;
        }


        if(touch_x > 100 && touch_x < 200 && touch_y > 40 && touch_y < 140)
        {
            touch_x = 0;
            touch_y = 0;
            if(icon_num_now < 1)
            {
                continue;
            }
            media_play(1);//参数是当前页的第几个视频
        }
        if(touch_x > 330 && touch_x < 430 && touch_y > 40 && touch_y < 140)
        {
            touch_x = 0;
            touch_y = 0;
            if(icon_num_now < 2)
            {
                continue;
            }
            media_play(2);//参数是当前页的第几个视频
        }
        if(touch_x > 560 && touch_x < 660 && touch_y > 40 && touch_y < 140)
        {
            touch_x = 0;
            touch_y = 0;
            if(icon_num_now < 3)
            {
                continue;
            }
            media_play(3);//参数是当前页的第几个视频
        }
        if(touch_x > 100 && touch_x < 200 && touch_y > 260 && touch_y < 360)
        {
            touch_x = 0;
            touch_y = 0;
            if(icon_num_now < 4)
            {
                continue;
            }
            media_play(4);//参数是当前页的第几个视频
        }
        if(touch_x > 330 && touch_x < 430 && touch_y > 260 && touch_y < 360)
        {
            touch_x = 0;
            touch_y = 0;
            if(icon_num_now < 5)
            {
                continue;
            }
            media_play(5);//参数是当前页的第几个视频
        }
        if(touch_x > 560 && touch_x < 660 && touch_y > 260 && touch_y < 360)
        {
            touch_x = 0;
            touch_y = 0;
            if(icon_num_now < 6)
            {
                continue;
            }
            media_play(6);//参数是当前页的第几个视频
        }
    }
}