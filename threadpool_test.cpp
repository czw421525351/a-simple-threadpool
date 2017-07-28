//
// Created by 17723 on 2017/5/17.
//

#include "threadpool.h"
#include <unistd.h>

//任务函数
void* task_routine1(void* args)
{
    int* pdata = (int*)args;
    printf("----------thread%ld----------working on task%d\n",pthread_self(),*pdata);
    sleep(5);
    printf("----------thread%ld----------finish task%d----\n",pthread_self(),*pdata);
    delete pdata;//销毁参数
    return NULL;
}

void* task_routine2(void* args,int data2,int data3)
{
    int* pdata = (int*)args;
    printf("----------thread%ld----------working on task%d data1=%d data2=%d \n",pthread_self(),*pdata,data2,data3);
    sleep(5);
    printf("----------thread%ld----------finish task%d----\n",pthread_self(),*pdata);
    delete pdata;//销毁参数
    return NULL;
}


int main()
{
    ThreadPool threadPool(5);

    for (int i = 0; i < 5; ++i)
    {
        int* pdata = new int(i);
        threadPool.threadpool_add_task(bind(task_routine1,(void*)pdata));
    }

    for(int i = 5; i < 10; ++i)
    {
        int data1 = i+100;
        int data2 = i+200;
        int* pdata = new int(i);
        threadPool.threadpool_add_task(bind(task_routine2,(void*)pdata,data1,data2));
    }
    sleep(30);
    threadPool.threadpool_destroy();
    return 0;
}