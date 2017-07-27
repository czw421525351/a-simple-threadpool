//
// Created by 17723 on 2017/5/16.
//

#ifndef SOCKET_PROGRAM_THREADPOOL_H
#define SOCKET_PROGRAM_THREADPOOL_H

#include <queue>
#include "condition.h"

struct Task
{
private:
    typedef void* (*TaskRoutine)(void* args);   //修改为用boost:：function来写
    typedef void* Args;
public:
    Task(TaskRoutine taskRoutine, Args args):taskRoutine_(taskRoutine),\
                                                args_(args){}
    TaskRoutine taskRoutine_;
    Args args_;
};

class ThreadPool
{
public:
    typedef void* (*TaskRoutine)(void* args);
    typedef void* Args;

    ThreadPool(int maxThreads = 3):counter(0),idle(0),\
                            maxThreads_(maxThreads),\
                                   condition(),taskQueue(),quit(false){}

    unsigned int counter;       //当前池中有多少线程
    unsigned int idle;          //空闲线程数
    unsigned int maxThreads_;   //线程数阈值
    bool quit;

    Condition condition;
    std::queue<Task*> taskQueue;

    void threadpool_add_task(TaskRoutine, Args); //这不能这样写 用Task就好
    void threadpool_destroy();

    void thread_detach(pthread_t);
    static void* threadRoutine(void*);           //这里不能写成成员函数，因为第一个参数会变成this指针
};


#endif //SOCKET_PROGRAM_THREADPOOL_H
