//
// Created by 17723 on 2017/5/16.
//

#ifndef SOCKET_PROGRAM_CONDITION_H
#define SOCKET_PROGRAM_CONDITION_H

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <pthread.h>


inline void PEXIT_FAILURE(int state)
{
    fprintf(stderr,"%s : %s\n",__func__,strerror(state));
    pthread_exit(NULL);
}

class Condition
{
public:
    Condition():mutex(PTHREAD_MUTEX_INITIALIZER),\
                cond(PTHREAD_COND_INITIALIZER){}
    void condition_lock();
    void condition_unlock();
    void condition_wait();
    int condition_timewait(const struct timespec);
    void condition_signal();
    void condition_broadcast();
    ~Condition();
private:
    void condition_destory();
private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

#endif //SOCKET_PROGRAM_CONDITION_H
