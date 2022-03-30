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
    server.init(config.PORT, user, passwd, databasename, config.LOGWrite, config.OPT_LINGER, config.TRIGMode, config.sql_num, config.thread_nums, config.close_log, config.actor_model);
    
    server.log_write();
    server.sql_pool();
    server.thread_pool();
    server.trig_mode();
    server.eventListen();
    server.eventLoop();
    return 0;
}
