//
// Created by 17723 on 2017/5/17.
//

#include "threadpool.h"
#include <unistd.h>

//任务函数
void* task_routine(void* args)
{
    int* pdata = (int*)args;
    sleep(4);
    delete pdata;//销毁参数
    return NULL;
}

int main()
{
    ThreadPool threadPool(5);

    for (int i = 0; i < 20; ++i) {

        int* pdata = new int(i);
        threadPool.threadpool_add_task(task_routine,(void*)pdata);

    }
    //sleep(50);
    threadPool.threadpool_destroy();
    return 0;
}