#ifndef __MYSQL_CLIENT_H__
#define __MYSQL_CLIENT_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include "mysql/mysql.h"
#include "log.h"
#include "noncopyable.h"

class CMysqlClient : private noncopyable {
public:
    CMysqlClient();
    virtual ~CMysqlClient();

public:
    //初始化
    int32_t init(const char* host, int32_t port, const char* dbname, const char* user, const char* pwd);
    //关闭连接
    int32_t close();
    //查询操作
    int32_t query(const char* sql);
    //更新操作
    int32_t exec(const char* sql);
    //重置查询缓存
    void reset_query();
    //获取单行记录
    MYSQL_ROW fetch_row();
    //获取记录长度
    uint64_t* fetch_lengths();
    //转义字符处理
    uint64_t escape_string(char* to, const char* from, uint64_t length);
    //获取字段值
    const char* fetch_field(MYSQL_ROW row, MYSQL_RES* res, const char* name, int32_t* len);
    //错误编号
    int32_t get_errno();
    //错误描述
    const char* get_errmsg();

private:
    MYSQL*      m_mysql;
    MYSQL_RES*  m_res;
    int         m_errno;
    std::string m_errmsg;
};

#endif
