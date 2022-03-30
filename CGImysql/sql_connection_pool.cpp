/*************************************************************************
	> File Name: sql_connection_pool.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 22 Mar 2022 05:10:28 PM CST
 ************************************************************************/
#include <mysql/mysql.h>
#include "./sql_connection_pool.h"

connection_pool::connection_pool() : m_CurConn(0), m_FreeConn(0) {}

connection_pool *connection_pool::GetInstance() {
    static connection_pool connPool;
    return &connPool;
}

void connection_pool::init(string url, string User, string PassWord, string DBName, int Port, int MaxConn, int close_log) {
    m_url = url;
	m_Port = Port;
	m_User = User;
	m_PassWord = PassWord;
	m_DatabaseName = DBName;
	m_close_log = close_log;

    for (int i = 0; i < MaxConn; i++) {
        MYSQL *con = nullptr;
        con = mysql_init(con);

        if (con == nullptr) {
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), Port, nullptr, 0);

        if (con == nullptr) {
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        connList.push_back(con);
        ++m_FreeConn;
    }

    reserve = sem(m_FreeConn);   // reserve is a sem
    m_MaxConn = m_FreeConn;
}

MYSQL *connection_pool::GetConnection() {
    MYSQL *con = nullptr;
    if (0 == connList.size()) return nullptr;
    
    reserve.wait();     //reserve-- 0 block >0 goto next
    lock.lock();
    con = connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;
    lock.unlock();
    return con;
}

bool connection_pool::ReleaseConnection(MYSQL *con) {
    if (NULL == con) return false;
    lock.lock();

    connList.push_back(con);   // push back in the pool
    ++m_FreeConn;
    --m_CurConn;

    lock.unlock();
    reserve.post();     // reverse++ inform wait()
    return true;
}

void connection_pool::DestroyPool() {
    lock.lock();
    if (connList.size() > 0) {
        list<MYSQL *>::iterator it;
        for (it = connList.begin(); it != connList.end(); ++it) {
            MYSQL *con = *it;
            mysql_close(con);
        }
        m_CurConn = 0;
        m_FreeConn = 0;
        connList.clear();
    }
    lock.unlock();
}

int connection_pool::GetFreeConn() {
    return this->m_FreeConn;
}

connection_pool::~connection_pool() {
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool) {
    *SQL = connPool->GetConnection();
    conRAII = *SQL;
    poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
    poolRAII->ReleaseConnection(conRAII);
}

