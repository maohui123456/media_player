#include "project.h"

int fifo_fd;    //创建有名管道向mplayer输入命令
int pipe_fd[2]; //创建无名管道专门用来获取mplayer的信息
pid_t pid;
bool close_flag;
bool error_flag;//打开视频出错的时候，这个标志置1
int pause_flag;//0代表没有暂停，1代表暂停
int ret_flag;
int prev_precent;//改变前播放的百分比
int sec = 0;//当前播放的秒数
int precent;//当前播放的百分比
pthread_t id1;
pthread_t id2;
pthread_t id3;
pthread_t id4;
float media_total_time ;
#define NEXT 1
#define PREV -1
#define NONE 0
//有名管道子进程和父进程都要打开，由于子进程是运行mplayer的，所以用有名管道，子进程的有名管道写端不用，只用读端
//mplayer -quiet -slave -zoom -x 800 -y 380  -input file=/tmp/pipe这句话只是告诉mplayer跟哪个有名管道相联系，而并没有打开有名管道
//而如果是用popen的话，在打开mplayer的同时也打开了管道，所以如果用popen来做的话，在子进程就不用重新打开管道了，但是父进程一定要
//而mplayer输出的东西在stdout中，我们可以在父进程中开一个线程来读stdout的东西，然后再开一个线程往有名管道里面写命令
void * get_media_info(void * arg)
{
    char buf[100];
    close(pipe_fd[1]);//关掉父进程的无名管道的写端
    int size = 0;
    while(1)
    {
        size = read(pipe_fd[0] , buf , sizeof(buf));//从已经导向stdout的读端，读出stdout的内容，因为mplayer会把所有信息输出在stdout中
        buf[size] = '\0';//读出来的东西不是字符串,跑到读到的东西的最后一个字节加上\0
        printf("read from mplayer %s\n" , buf);//输出读到的东西
        if(strstr(buf , "Exiting... (End of file)") != NULL)//因为视频错误而打开失败
        {
            error_flag = true;
            printf("打开视频出错\n");  
        }
        else
        {
            error_flag = false;
        }
        if(strstr(buf , "ANS_LENGTH=") != NULL)//这里如果mplayer输出的是总视频时长，那么他会把输出的内容转化为数字保存在media_total_time里
        {
            //printf("%s\n" , buf);
            char * middle_buf = strchr(buf , '=');
            middle_buf+=1;
            media_total_time = atof(middle_buf);
            //printf("%f\n" , media_total_time);
        }
        if(strstr(buf , "ANS_PERCENT_POSITION=") != NULL)//输出的是当前播放的百分比，保存在precent里面
        {
            //printf("%s\n" , buf);
            char * middle_buf = strchr(buf , '=');
            middle_buf+=1;
            precent = atoi(middle_buf);
            // printf("%d\n" , precent);
        }
    }
}

void * write_media_info(void * arg)
{
    while(1)
    {
        /*=========================================
        
        下面这段是在slave模式下通过键盘输入命令的范例
        
        ========================================*/
        // char buf[100];
        // fflush(stdout);//做这个之前先把标准输出的东西清干净
        // pthread_mutex_lock(&my_lock);
        // pthread_cond_wait(&my_cond , &my_lock);
        // fgets(buf , sizeof(buf) , stdin);//从标准输入里面获得输入的东西，就是你键盘输入的东西，如果你键盘没有输入，它会阻塞等待，然后放在buf里面
        // buf[strlen(buf)] = '\0';//同样，往最后面加一个\0形成字符串
        // printf("*%s*\n" , buf);
        

        /*=========================================
        
        下面这段通过触摸屏输入指令
        
        ========================================*/
        if(touch_x > 345 && touch_x < 427 && touch_y > 400 && touch_y < 480)//pause
        {
            touch_x = 0;
            touch_y = 0;
            if(error_flag == false)
            {
                if(write(fifo_fd , "pause\n" , strlen("pause\n")) != strlen("pause\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            } 
            if(pause_flag == 0)
            {
                pause_flag = 1;
            }
            else
            {
                pause_flag = 0;
            }

        }

        if(touch_x > 0 && touch_x < 100 && touch_y > 400 && touch_y < 480)//退出
        {
            touch_x = 0;
            touch_y = 0;
            if(error_flag == false)
            {
                if(write(fifo_fd , "quit\n" , strlen("quit\n")) != strlen("quit\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            }
            close_flag = true;
            ret_flag = NONE;
            precent = 0;
            show_bmp_according_x_y(mmap_p , MEDIA_CONTROL_PIC , 0 , 0 , 800 , 480);
            pthread_cancel(id3);
            kill(pid, SIGKILL);
            pages = 1;
            icon_num = avi_num;
            show_bmp_according_x_y(mmap_p , BRACKGROUND_BMP , 0 , 0 , 800 , 480 );
            show_howmany_icon();
        }

        if(touch_x > 230 && touch_x < 310 && touch_y > 400 && touch_y < 480)//快退10s
        {
            touch_x = 0;
            touch_y = 0;
            if(error_flag == false)
            {
                if(write(fifo_fd , "seek -10  0\n" , strlen("seek +10  0\n")) != strlen("seek +10  0\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            }
        }

        if(touch_x > 460 && touch_x < 545 && touch_y > 400 && touch_y < 480)//快进10s
        {
            touch_x = 0;
            touch_y = 0;
            //precent++;
            if(error_flag == false)
            {
                if(write(fifo_fd , "seek +10  0\n" , strlen("seek +10  0\n")) != strlen("seek +10  0\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            }
        }

        if(touch_x > 670 && touch_x < 730 && touch_y > 400 && touch_y < 480)//音量+
        {
            touch_x = 0;
            touch_y = 0;
            if(error_flag == false)
            {
                if(write(fifo_fd , "volume +1\n" , strlen("volume +1\n")) != strlen("volume +1\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            }
        }

        if(touch_x > 730 && touch_x < 800 && touch_y > 400 && touch_y < 480)//音量-
        {
            touch_x = 0;
            touch_y = 0;
            if(error_flag == false)
            {
                if(write(fifo_fd , "volume -1\n" , strlen("volume -1\n")) != strlen("volume -1\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            }
        }

        if(touch_x > 545 && touch_x < 630 && touch_y > 400 && touch_y < 480)//下一个视频
        {
            touch_x = 0;
            touch_y = 0;
            close_flag = true;
            pause_flag = 0;
            //找当前播放的视频的名字，返回给主线程，主线程返回给上一个函数，返回指令，是上一个视频还是下一个视频
            ret_flag = NEXT;
            if(error_flag == false)
            {
                if(write(fifo_fd , "quit\n" , strlen("quit\n")) != strlen("quit\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            }
            show_bmp_according_x_y(mmap_p , MEDIA_CONTROL_PIC , 0 , 0 , 800 , 480);
            pthread_cancel(id3);
            kill(pid, SIGKILL);
            show_bmp_according_x_y(mmap_p , MEDIA_CONTROL_PIC , 0 , 0 , 800 , 480);
            precent = 0;
        }

        if(touch_x > 150 && touch_x < 230 && touch_y > 400 && touch_y < 480)//上一个视频
        {
            touch_x = 0;
            touch_y = 0;
            close_flag = true;
            pause_flag = 0;
            //找当前播放的视频的名字，返回给主线程，主线程返回给上一个函数，返回指令，是上一个视频还是下一个视频
            ret_flag = PREV;
            if(error_flag == false)
            {
                if(write(fifo_fd , "quit\n" , strlen("quit\n")) != strlen("quit\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                perror("write ");
            }
            show_bmp_according_x_y(mmap_p , MEDIA_CONTROL_PIC , 0 , 0 , 800 , 480);
            pthread_cancel(id3);
            kill(pid, SIGKILL);
            show_bmp_according_x_y(mmap_p , MEDIA_CONTROL_PIC , 0 , 0 , 800 , 480);
            precent = 0;
        }

        if(touch_x > 0 && touch_x < 800 && touch_y > 200 && touch_y < 400)//通过进度条来跳
        {
            if(pressure == 0)
            {
                char seek_buf[50];
                float seek_where = ((float)touch_x/800)*100;
                touch_x = 0;
                touch_y = 0;
                printf("seek = %d\n" , (int)seek_where);
                sprintf(seek_buf , "%s %d %d\n" , "seek" , (int)seek_where , 1);
                printf("%s\n" , seek_buf);
                if(error_flag == false)
                {
                    if(write(fifo_fd , seek_buf , strlen(seek_buf)) != strlen(seek_buf));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
                    perror("write ");
                }
                printf("pressure = %d\n" , pressure);
            }
            
            
        }

        if(close_flag)//释放资源
        {
            pthread_cancel(id1);
            //kill(pid, SIGKILL);
            printf("子进程退出\n");
            close(fifo_fd);
            close(pipe_fd[0]);
            pthread_exit((void *)&ret_flag);   
        }
    } 
}

void * process(void * arg)//进度条
{
    //每1000毫秒就向mplayer获得一次进度百分比
    while(1)
    {
        usleep(10000);
        // sleep(1);
        if(pause_flag == 1)
        continue; 
        if(error_flag == false)//发信号到管道获得当前百分比
        {
            if(write(fifo_fd , "get_percent_pos\n" , strlen("get_percent_pos\n")) != strlen("get_percent_pos\n"));//把buf里的字符串输入到有名管道中，然后输入给mplayer//特别注意命令后面一定要有\n
            //perror("write ");
        }
        if(prev_precent < precent || precent == 0)//如果是向前倒退，或者当前百分比是0
        {
            for(int i = 384 ; i < 398 ; i++)
            {
                for(int j = 0 ; j < 800 ; j++)
                {
                    mmap_p[800*i + j ] = 0xffffff;
                }
            }
        }
        for(int i = 380 ; i < 400 ; i++)
        {
            for(int j = 0 ; j < (800*precent)/100 ; j++)
            {
                mmap_p[800*i + j ] = 0x000000;
            }
        }
        prev_precent = precent;
    }   
}


int control_media(char * avi_name_buf)
{
    //创建有名管道
    int fifo_ret = mkfifo(FIFO_PATH , 0777);
    show_bmp_according_x_y(mmap_p , MEDIA_CONTROL_PIC , 0 , 0 , 800 , 480);
    close_flag = false;
    if(fifo_ret == -1)
    {
        if(errno == EEXIST)//如果存在，删掉重新创建
        {
            unlink(FIFO_PATH);
            mkfifo(FIFO_PATH , O_CREAT|0777);
        }
        else
        {
            perror("创建有名管道失败\n");
            return 3;
        }
    }
    //创建无名管道
    if(pipe(pipe_fd) < 0)
    {
        perror("创建无名管道失败\n");
        return 2;
    }

    pid = fork();//创建新的进程
    if(pid == 0)//子进程，主要任务是播放mplayer并把它的输出传到无名管道中
    {
        close(pipe_fd[0]);//首先把无名管道的读端关掉
        dup2(pipe_fd[1] , 1);//把管道的写端导入到stdin标准输出中，标准输出的fd是1，dup2函数的作用就是把参数2的fd复制到参数1那里去，参数1也有参数2的功能，这里把管道输出跟stdin标准输出联系起来
        fifo_fd = open(FIFO_PATH , O_RDWR);//打开有名管道
        system(avi_name_buf);//播放视频
    }
    else//父进程，创建两个线程，一个用来获取无名管道传过来的信息，一个负责向有名管道传信息
    {
        void * retval = NULL;
        error_flag = false;
        fifo_fd = open(FIFO_PATH , O_RDWR);//打开有名管道
        //创建两个线程
        pthread_create(&id1 , NULL , get_media_info , NULL);
        pthread_create(&id2 , NULL , write_media_info , avi_name_buf);
        pthread_create(&id3 , NULL , process , NULL);
        //等待线程退出
        pthread_join(id1 , NULL);
        printf("线程1退出\n");
        pthread_join(id2 , &retval);
        printf("线程2退出\n");
        pthread_join(id3 , NULL);
        printf("线程3退出\n");
        printf("retval = %d\n" , *(int *)retval );
        return *(int *)retval;
    }
}