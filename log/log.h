/*************************************************************************
	> File Name: log.h
	> Author: 
	> Mail: 
	> Created Time: Wed 23 Mar 2022 04:34:31 PM CST
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <pthread.h>
#include "block_queue.h"

using namespacee std;

class Log {
public:
    static Log *get_instance() {
        static Log instance;
        return &instance;
    }
    static void *flush_log_thread(void *args) {
        Log::get_instance()->async_write_log();
    }
    bool init(const char *file_name, int close_log, int log_buf_size = 8192, int split_line = 5000000, int max_queue_size = 0);
    void write_log(int level, const char *format, ...);
    void flush(void);

private:
    Log();
    virtual ~Log();
    void *async_write_log() {
        string single_log;
        while (m_log_queue->pop(single_log)) {
            m_mutex.lock();
            fputs(single_log.c_str(), m_fp);
            m_mutex.unlock();
        }
    }

private:
    char dir_name[128];
    char log_name[128];
    int m_split_lines;     // log file max line
    int m_log_buf_size;
    long long m_count;
    int m_today;           // creat log file by day
    FILE *m_fp;
    char *m_buf;
    block_queue<string> *m_log_queue;
    bool m_is_async;
    locker m_mutex;
    int m_close_log;
};

#define LOG_DEBUG(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(0, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(1, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(2, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(3, format, ##__VA_ARGS__); Log::get_instance()->flush();}




#endif