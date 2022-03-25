/*************************************************************************
	> File Name: time_wheel.h
	> Author: 
	> Mail: 
	> Created Time: Fri 25 Mar 2022 04:41:16 PM CST
 ************************************************************************/

#ifndef _TIME_WHEEL_H
#define _TIME_WHEEL_H

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>

#include <time.h>
#include "../log/log.h"

class tw_timer;
struct client_data {
    sockaddr_in address;
    int sockfd;
    tw_timer* timer;
};

class tw_timer {
public:
    tw_timer( int rot, int ts )
    : next( NULL ), prev( NULL ), rotation( rot ), time_slot( ts ){}

public:
    int rotation;
    int time_slot;
    void (*cb_func)( client_data* );
    client_data* user_data;
    tw_timer* next;
    tw_timer* prev;
};

class time_wheel {
public:
    time_wheel();
    ~time_wheel();
    tw_timer* add_timer( int timeout );
    void del_timer( tw_timer* timer );
    void tick();

private:
    static const int N = 60;
    static const int TI = 1;
    tw_timer* slots[N];
    int cur_slot;
};

class Utils {
public:
    Utils() {}
    ~Utils() {}

    void init(int timeslot);
    int setnonblocking(int fd);
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);
    static void sig_handler(int sig);
    void addsig(int sig, void(handler)(int), bool restart = true);
    void timer_handler();
    void show_error(int connfd, const char *info);
    
public:
    static int *u_pipefd;
    time_wheel m_time_wheel;
    static int u_epollfd;
    int m_TIMESLOT;

};

void cb_func(client_data *user_data);

#endif
