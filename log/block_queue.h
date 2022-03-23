/*************************************************************************
	> File Name: block_queue.h
	> Author: 
	> Mail: 
	> Created Time: Wed 23 Mar 2022 05:33:12 PM CST
 ************************************************************************/

#ifndef _BLOCK_QUEUE_H
#define _BLOCK_QUEUE_H

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "../lock/locker.h"
using namespace std;

template<typename T>
class block_queue {
public:
    block_queue(int max_size = 1000) {}

private:
    locker m_mutex;
    cond m_cond;
    
    T *m_array;
    int m_size;
    int m_max_size;
    int m_front;
    int m_back;
};

#endif
