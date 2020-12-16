#include "project.h"

void tail_insert(struct avi_name_list * name_head , struct avi_name_list * new_node)
{
    struct avi_name_list * p = name_head;
    while(p->next != name_head)//找到最后一个节点
    {
        p = p->next; 
    }
    new_node->next = name_head;
    new_node->prev = p;
    p->next = new_node;
    name_head->prev = new_node;
    avi_num++;
}

struct avi_name_list * creat_head()
{
    struct avi_name_list * name_head = malloc(sizeof(struct avi_name_list));
    if(name_head == NULL)
    {
        perror("创建头节点失败\n");
        return NULL;
    }
    name_head->next = name_head;
    name_head->prev = name_head;
    return name_head;
}

struct avi_name_list * creat_new_node(char * name)
{
    
    struct avi_name_list * new_node = malloc(sizeof(struct avi_name_list));
    bzero(new_node , sizeof(struct avi_name_list));
    if(new_node == NULL)
    {
        perror("创建新节点失败\n");
        return NULL;
    }
    
    strcpy(new_node->name , name);
    //printf("%s\n" , new_node->name);
    new_node->next = NULL;
    new_node->prev = NULL;
    
    return new_node;
}





struct avi_name_list * get_avi_name()
{
    avi_num = 0;
    struct dirent * dir_info;
    struct avi_name_list * name_head = creat_head();
    DIR * dp = opendir("./avi");
    if(dp == NULL)
    {
        perror("打开目录失败\n");
    }
    while(1)
    {
        dir_info = readdir(dp);
        if(dir_info == NULL)
        {
            printf("目录已经读完\n");
            break;
        }
        if(dir_info->d_type == DT_REG)
        {
            char * strstr_ret = strstr(dir_info->d_name , ".avi");
            if(strstr_ret == NULL)
            {
                continue;
            }
            //printf("%s\n" , dir_info->d_name);
            struct avi_name_list * new_node = creat_new_node(dir_info->d_name);
            tail_insert(name_head , new_node);
        }
    }
    
    free(dir_info);
    return name_head;
}