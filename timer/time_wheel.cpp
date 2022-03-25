/*************************************************************************
	> File Name: time_wheel.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 25 Mar 2022 04:44:48 PM CST
 ************************************************************************/

#include "time_wheel.h"
#include "../http/http_conn.h"
time_wheel::time_wheel() : cur_slot( 0 ) {
    for( int i = 0; i < N; ++i ) {
        slots[i] = NULL;
    }
}
time_wheel::~time_wheel() {
    for( int i = 0; i < N; ++i ) {
        tw_timer* tmp = slots[i];
        while( tmp ) {
            slots[i] = tmp->next;
            delete tmp;
            tmp = slots[i];
        }
    }
}

tw_timer* time_wheel::add_timer( int timeout ) {
    if( timeout < 0 ) {
        return NULL;
    }
    int ticks = 0;
    
    if( timeout < TI ) {
        ticks = 1;
    } else {
        ticks = timeout / TI;
    }
    
    int rotation = ticks / N;
    int ts = ( cur_slot + ( ticks % N ) ) % N;
    tw_timer* timer = new tw_timer( rotation, ts );
    if( !slots[ts] ) {
        printf( "add timer, rotation is %d, ts is %d, cur_slot is %d\n", rotation, ts, cur_slot );
        slots[ts] = timer;
    } else {
        timer->next = slots[ts];
        slots[ts]->prev = timer;
        slots[ts] = timer;
    }
    return timer;
}
void time_wheel::del_timer( tw_timer* timer ) {
    if( !timer ) {
        return;
    }
    int ts = timer->time_slot;
    if( timer == slots[ts] ) {
        slots[ts] = slots[ts]->next;
        if( slots[ts] ) {
            slots[ts]->prev = NULL;
        }
        delete timer;
    } else {
        timer->prev->next = timer->next;
        if( timer->next ) {
            timer->next->prev = timer->prev;
        }
        delete timer;
    }
}
void time_wheel::tick() {
    tw_timer* tmp = slots[cur_slot];
    printf( "current slot is %d\n", cur_slot );
    while( tmp ) {
        printf( "tick the timer once\n" );
        if( tmp->rotation > 0 ) {
            tmp->rotation--;
            tmp = tmp->next;
        } else {
            tmp->cb_func( tmp->user_data );
            if( tmp == slots[cur_slot] ) {
                printf( "delete header in cur_slot\n" );
                slots[cur_slot] = tmp->next;
                delete tmp;
                if( slots[cur_slot] ) {
                    slots[cur_slot]->prev = NULL;
                }
                tmp = slots[cur_slot];
            } else {
                tmp->prev->next = tmp->next;
                if( tmp->next ) {
                    tmp->next->prev = tmp->prev;
                }
                tw_timer* tmp2 = tmp->next;
                delete tmp;
                tmp = tmp2;
            }
        }
    }
    cur_slot = ++cur_slot % N;
}

void Utils::init(int timeslot) {
    m_TIMESLOT = timeslot;
}

int Utils::setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void Utils::addfd(int epollfd, int fd, bool one_shot, inti TRIGMode) {
    epoll_event event;
    event.data.fd = fd;
    if (1 == TRIGMode) event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot) event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

void Utils::sig_handler(int sig) {
    int save_errno = errno;
    int msg = sig;
    send(u_pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

void Utils::addsig(int sig, void(handler)(int), bool restart) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    if (restart) sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void Utils::timer_handler() {
    m_time_wheel.tick();
    alarm(m_TIMESLOT);
}

void Utils::show_error(int connfd, const char *info) {
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int *Utils::u_pipefd = 0;
int Utils::u_epollfd = 0;

class Utils;
void cb_func(client_data *user_data) {
    epoll_ctl(Utils::u_epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    http_conn::m_user_count--;
}

