/*************************************************************************
	> File Name: threadpool.h
	> Author: 
	> Mail: 
	> Created Time: Tue 22 Mar 2022 03:22:09 PM CST
 ************************************************************************/

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "../lock/locker.h"
#include "../CGImysql/sql_connection_poll.h"

template<typename T>
class threadpool {
public:
    threadpool(int actor_model, connection_pool *connPool, int thread_number = 8, int max_requests = 10000);
    ~threadpool();
    bool append(T *request, int state);
    bool append_p(T *request);

private:
    static void *worker(void *arg);
    void run();

private:
    int m_thread_number;
    int m_max_requests;
    pthread_t *m_threads;
    std::list<T *> m_workqueue;
    sem m_queuestat;
    connection_pool *m_connPool;
    int m_actor_model;
};

template<typename T>
threadpool<T>::threadpool(int actor_model, connection_pool *connPool, int thread_number, int max_requests) :

#endif
