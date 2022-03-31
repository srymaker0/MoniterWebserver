/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 29 Mar 2022 10:18:18 PM CST
 ************************************************************************/

#include "config.h"

int main(int argc, char *argv[]) {
    string user = "root";
    string passwd = "ZYJadmin@123";
    string databasename = "srydb";

    Config config;
    config.parse_arg(argc, argv);
    WebServer server;
    printf("server init...\n");
    
    server.init(config.PORT, user, passwd, databasename, config.LOGWrite, config.OPT_LINGER, config.TRIGMode, config.sql_num, config.thread_nums, config.close_log, config.actor_model);
    
    //printf("log write...\n");
    server.log_write();

    //printf("sql pool...\n");
    server.sql_pool();

    //printf("thread pool...\n");
    server.thread_pool();

    //printf("trig mode...\n");
    server.trig_mode();

    //printf("eventListen...\n");
    server.eventListen();
    
    printf("eventLoop...\n");
    server.eventLoop();
    //printf("eventLoop after........\n");
    return 0;
}
