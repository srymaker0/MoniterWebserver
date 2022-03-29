/*************************************************************************
	> File Name: sql_connection_pool.h
	> Author: 
	> Mail: 
	> Created Time: Tue 22 Mar 2022 04:31:27 PM CST
 ************************************************************************/

#ifndef _SQL_CONNECTION_POOL_H
#define _SQL_CONNECTION_POOL_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include "../lock/locker.h"
#include "../log/log.h"

using namespace std;

class connection_pool {
public:
    MYSQL *GetConnection();
    bool ReleaseConnection(MYSQL *conn);
    int GetFreeConn();
    void DestroyPool();
    void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log);
private:
    connection_pool();
    ~connection_pool();

    int m_MaxConn;
    int m_CurConn;
    int m_FreeConn;
    locker lock;
    list<MYSQL *> connList;
    sem reserve;

public:
    string m_url;
    string m_Port;
    string m_User;
    string m_PassWord;
    string m_DatabaseName;
    int m_close_log;
};

class connectionRAII {
public:
    connectionRAII(MYSQL **con, connection_pool *connPool);
    ~connectionRAII();

private:
    MYSQL *connRAII;
    connection_pool *poolRAII;
};


#endif
