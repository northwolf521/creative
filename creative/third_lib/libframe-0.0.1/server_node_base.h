#ifndef __SERVER_NODE_BASE_H__
#define __SERVER_NODE_BASE_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <map>
#include "log.h"
#include "event2/event.h"
#include "event2/event_compat.h"
#include "event2/event_struct.h"

typedef void (*signal_fn)(evutil_socket_t sig, short events, void *arg);

class server_node_base {
public:
    server_node_base();
    virtual ~server_node_base();

    //服务启动
    virtual int32_t init(int32_t argc, char** argv);
    //服务循环
    virtual int32_t loop(int32_t flags = 0);
    //服务终止
    virtual int32_t final();
    //终止服务操作
    virtual int32_t stop();

public:
    //获取时间函数
    int32_t gettimeofday_cached(struct timeval* tv);

protected:
    //捕获系统信号
    int32_t add_signal_handler(signal_fn fn, int32_t signo, void* pridata);
    //释放系统信号
    int32_t del_signal_handler(int32_t signo);
    //事件管理对象
    struct event_base* event_base_ptr();
    //添加周期回调函数
    int32_t add_period_cb(struct event* ev, int32_t ms, event_callback_fn fn, void* arg);
    //添加超时回调函数
    int32_t add_timeout_cb(struct event* ev, int32_t ms, event_callback_fn fn, void* arg);

protected:
    //事件管理
    struct event_base* _evbase;
    //管理配置
    struct event_config* _evconfig;
    //信号事件
    std::map<int32_t, struct event*> _signal_map;
};

#endif