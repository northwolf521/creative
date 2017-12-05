#ifndef __ASYNC_REDIS_CLIENT_H__
#define __ASYNC_REDIS_CLIENT_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "event2/event.h"
#include "event2/event_compat.h"
#include "event2/event_struct.h"
#include "log.h"
#include "async.h"
#include "hiredis.h"

class async_redis_client {
public:
    async_redis_client(struct event_base* evbase);
    virtual ~async_redis_client();

    //主机地址
    void set_addr(const char* addr);
    //认证密码
    void set_pass(const char* pass);
    //主机端口
    void set_port(const int32_t port);
    //区服编号
    void set_partid(const int32_t partid);
    //建立连接
    int32_t connect();
    //连接成功通知
    void cb_connected();
    //断开连接通知
    void cb_disconnected();
    //执行命令
    int32_t cmd(redisCallbackFn *fn, void *privdata, const char *format, ...);
    //是否连接状态
    int32_t is_connected();
    //断开连接
    void disconnect();
    //关联分区
    int32_t get_partid();

protected:
    struct event_base* _evb;
    redisAsyncContext* _ctx;
    char _addr[64];
    char _pass[64];
    int32_t _port;
    int32_t _partid;
    int32_t _linkid;
};

#endif

