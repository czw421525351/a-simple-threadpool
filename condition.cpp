//
// Created by 17723 on 2017/5/16.
//

#include "condition.h"

void Condition::condition_lock() {
    int state;
    if((state = pthread_mutex_lock(&mutex)) != 0 )
        PEXIT_FAILURE(state);
}

void Condition::condition_unlock() {
    int state;
    if((state = pthread_mutex_unlock(&mutex)) != 0)
        PEXIT_FAILURE(state);
}

void Condition::condition_signal() {
    int state;
    if((state = pthread_cond_signal(&cond)) != 0)
        PEXIT_FAILURE(state);
}

void Condition::condition_broadcast() {
    int state;
    if((state = pthread_cond_broadcast(&cond))!= 0)
        PEXIT_FAILURE(state);
}
void Condition::condition_wait() {
    int state;
    if((state = pthread_cond_wait(&cond,&mutex))!= 0)
        PEXIT_FAILURE(state);
}

int Condition::condition_timewait(const timespec abstime) {

    return pthread_cond_timedwait(&cond,&mutex,&abstime);

}

void Condition::condition_destory() {
    int state;
    if ((state = pthread_cond_destroy(&cond)) != 0)
        PEXIT_FAILURE(state);
    if ((state = pthread_mutex_destroy(&mutex)) != 0)
        PEXIT_FAILURE(state);
}

Condition::~Condition() {

    condition_destory();
}
