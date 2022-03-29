/*************************************************************************
	> File Name: config.h
	> Author: 
	> Mail: 
	> Created Time: Tue 29 Mar 2022 10:22:26 PM CST
 ************************************************************************/

#ifndef _CONFIG_H
#define _CONFIG_H

#include "webserver.h"
using namespace std;

class Config {
public :
    Config();
    ~Config(){};
    
    void parse_arg(int argc, char *argv[]);

    int PORT;
    int LOGWrite;
    int TRIGMode;
    int LISTENTrigmode;
    int CONNTrigmode;
    int OPT_LINGER;
    int sql_num;
    int thread_nums;
    int close_log;
    int actor_model;
}

#endif
