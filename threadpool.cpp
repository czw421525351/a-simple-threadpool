//
// Created by 17723 on 2017/5/16.
//

#include "threadpool.h"
#include <errno.h>

void* ThreadPool::threadRoutine(void *args)
{

    ThreadPool* pool = (ThreadPool*)(args);
    printf("----------thread%ld-----is starting-----\n",pthread_self());
    pool->thread_detach(pthread_self());


    timespec abstime;
    int timeout;

    while(1)
    {
        timeout = 0;//超时标记
        pool->condition.condition_lock();
        pool->idle++;
        while (pool->taskQueue.empty() && !pool->quit)
        {
            printf("----------thread%ld-----is waitting-----\n",pthread_self());
            //pool->condition.condition_wait();
            clock_gettime(CLOCK_REALTIME,&abstime);
            abstime.tv_sec += 3;                        //设定超时时间是3秒
            int status = pool->condition.condition_timewait(abstime);
            if(status == ETIMEDOUT)
            {
                printf("----------thread%ld-----is wait timed out-----\n",pthread_self());
                timeout = 1;
                break;
            }
        }
        //等待到条件，处于工作状态
        pool->idle--;
        if (!(pool->taskQueue.empty()))
        {         //等待到任务
            Task *ptask = pool->taskQueue.front();//取出任务
            pool->taskQueue.pop();                //弹出任务

            pool->condition.condition_unlock();   //处理任务之前 先解锁 因为等待到条件时自动获取了一把锁

            int task_args = *(int *) (ptask->args_);
            printf("----------thread%ld-----is working on task%d-----\n", pthread_self(), task_args);
            ptask->taskRoutine_(ptask->args_);    //执行任务
            printf("----------thread%ld-----has finished task%d-----\n", pthread_self(), task_args);
            delete (ptask);                       //销毁任务

            pool->condition.condition_lock();     //这里加锁是因为循环最后有解锁操作
        }
        if(pool->quit && pool->taskQueue.empty())  //等待到线程池销毁通知，且任务都执行完毕
        {
            pool->counter--;                       //当前线程数减1

            if(pool->counter == 0)
                pool->condition.condition_signal();      //用于唤醒主线程

            pool->condition.condition_unlock();    //break 之前要先解锁 因为等待到条件时自动获取了一把锁
            break;
        }
        if(timeout == 1 && pool->taskQueue.empty())//线程等待超时
        {
            pool->counter--;
            pool->condition.condition_unlock();
            break;
        }
        pool->condition.condition_unlock();
    }
    printf("----------thread%ld-----is exiting-----\n",pthread_self());
    return NULL;
}


void ThreadPool::threadpool_add_task(TaskRoutine taskRoutine, Args args) //改成boost：:function
{
    Task* ptask = new Task(taskRoutine,args);
    ptask->taskRoutine_ = taskRoutine;
    ptask->args_ = args;

    condition.condition_lock();    //加锁

    taskQueue.push(ptask);

    if(idle > 0)//如果有等待线程则唤醒
        condition.condition_signal();
    else if(counter < maxThreads_) //池中没有等待线程且线程数没有超过阈值则创建新的线程执行任务
        {
            pthread_t newThreadID;
            pthread_create(&newThreadID,NULL,threadRoutine,this);
            counter++;
        }

    condition.condition_unlock();//解锁
}

void ThreadPool::threadpool_destroy()
{

    if(quit == 1)                //意思是这个函数已经被调用过一次了，就不要再调用了，直接返回
        return;
    condition.condition_lock();
    quit = 1;
    if(counter > 0)
    {
        if(idle > 0)
            condition.condition_broadcast();//处于执行任务状态的线程不会受到广播
                                            //线程池需要等待执行任务中的线程全部退出
        while(counter > 0)                  //所以这里也需要等待条件变量 那么谁去唤醒他呢
            condition.condition_wait();
    }
    condition.condition_unlock();

}

void ThreadPool::thread_detach(pthread_t pid)
{
    int state;
    if((state = pthread_detach(pid)) != 0)
        PEXIT_FAILURE(state);
};


