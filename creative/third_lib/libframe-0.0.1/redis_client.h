#ifndef __REDIS_CLIENT_H__
#define __REDIS_CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "stl_relation.h"
#include "log.h"
#include "hiredis.h"
#include "noncopyable.h"

class CAutoRedisReply : private noncopyable {
public:
    CAutoRedisReply(redisReply* r);
    virtual ~CAutoRedisReply();

public:
    void set(redisReply* r);
    redisReply* get();

private:
    CAutoRedisReply(const CAutoRedisReply&);
    CAutoRedisReply operator = (const CAutoRedisReply&);

private:
    redisReply* _reply;
};

class CRedisClient : private noncopyable {
public:
    CRedisClient();
    virtual ~CRedisClient();

public:
    //清除数据
    void clear();
    //建立连接
    int connect(const char* host, int port, int timeout = 5000, const char* passwd = NULL);
    //检查连接有效性
    int check_connect();
    //哈希表中域的数量
    int count(const char* table);
    //查看哈希表 key 中，给定域 field 是否存在 >0-存在; 0-不存在, <0-错误
    int exist(const char* table, const char* key);
    //带键值插入操作
    int set(const char* table, const char* key, const char* val);
    //增加key的值
    int inc(const char* table, const char* key, uint32_t val = 1);
    //修改名称
    int rename(const char* table, const char* newtable);
    //获取多个键值
    int get(const char* table, const std::vector<uint64_t>& keys, std::vector<std::string>& out);
    //获取多个键值
    int get(const char* table, const std::vector<std::string>& keys, std::vector<std::string>& out);
    //带健值读取
    std::string get(const char* table, const char* key);
    //带健值读取
    int get(const char* table, const char* key, std::string& out);
    //带多键值删除
    int del(const char* table, const std::vector<uint64_t>& keys);
    //带多键删除
    int del(const char* table, const std::vector<std::string>& keys);
    //删除键值
    int del(const char* table, const char* key);
    //设置成员排行
    int zset(const char* key, uint64_t member, uint64_t score);
    //获取排行; 大从到小
    int zgetHL64(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint64_t> >& out);
    //获取排行; 大从到小
    int zgetHL(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint32_t> >& out);
    //获取排行; 大小到大
    int zgetLH64(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint64_t> >& out);
    //获取排行; 大小到大
    int zgetLH(const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint32_t> >& out);
    //获取成员的排名;从大到小
    int zget_rankHL(const char* key, uint64_t member, uint32_t& out);
    //获取成员的排名;从小到大
    int zget_rankLH(const char* key, uint64_t member, uint32_t& out);
    // 获取范围内score的个数  by lindayong
    int zcount(const char* key, int min, int max, uint32_t& out);
    //获取排行榜大小
    int zcard(const char* key, uint32_t& out);
    //获取成员的积分
    int zget_score64(const char* key, uint64_t member, uint64_t& out);
    //获取成员的积分
    int zget_score(const char* key, uint64_t member, uint32_t& out);
    //删除某人的排行
    int zdel(const char* key, const std::vector<uint64_t>& dellist);
    //增加key的值
    int zinc(const char* table, const char* key, uint32_t val = 1, uint64_t* newval = NULL);
    //删除某个表
    int del_table(const char* key);
    //链表操作(插入链表尾部)
    int list_insert(const char* key, const char* value);
    //链表操作(修改链表的值)
    int list_set(const char* key, uint32_t idx,  const char* value);
    //获取指定元素
    int list_get(const char* key, int startNo, int endNo, std::vector<std::string>& out);
    //从头部删除一个元素
    int list_header_pop(const char* key);
    //获取元素个数
    int list_size(const char* key);
    //全局加锁
    int lock(const char* key, const char* val);
    //全局解锁
    int unlock(const char* key);
    //获取所有键值
    int get_all_val(const char* table, std::vector<std::pair<std::string, std::string> >& out);
    //过期键值
    int expire_key(const char* key, int32_t seconds);

private:
    //建立连接
    int connect();
    //获取集合某个范围数据
    int zget64(bool rev, const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint64_t> >& out);
    //获取集合某个范围数据
    int zget32(bool rev, const char* key, int startNo, int endNo, std::vector<std::pair<uint64_t, uint32_t> >& out);
    //获取集合某个范围数据
    int zget_rank(bool rev, const char* key, uint64_t member, uint32_t& out);

private:
    bool          m_isConnect;
    redisContext* m_redisCtx;
    std::string   m_szHost;
    int           m_nPort;
    uint32_t      m_timeout;
    std::string   m_szPasswd;
};

#endif
