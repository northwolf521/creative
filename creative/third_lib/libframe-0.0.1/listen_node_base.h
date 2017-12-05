#ifndef __LISTEN_NODE_BASE_H__
#define __LISTEN_NODE_BASE_H__

#include <iostream>
#include <vector>
#include <map>

#include "conn_node_base.h"

class conn_node_base;
class listen_node_base {
public:
    listen_node_base(struct event_base* evbase);
    virtual ~listen_node_base();

    //接受连接检查
    virtual bool accept_check(struct sockaddr_in addr);
    //分配连接对象
    virtual conn_node_base* alloc_conn_node() = 0;
    //释放连接对象
    virtual void free_conn_node(conn_node_base* node) = 0;
    //关闭监听服务
    virtual void close();
    //开启监听服务
    virtual int  listen();
    //设置监听端口
    void set_port(uint16_t port);
    //获取监听端口
    uint16_t get_port();
    //设置监听主机信息
    int32_t set_hostinfo(const char* host);
    //释放监听对象
    static void close_conn_node(conn_node_base* conn_node);

private:
    //处理接受连接
    static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int salen, void* arg);
    //处理连接读取
    static void conn_readcb(struct bufferevent* bev, void* arg);
    //处理连接事件
    static void conn_eventcb(struct bufferevent* bev, short events, void* arg);

protected:
    //事件管理
    struct event_base* _evbase;

private:
    uint16_t m_port;
    char m_addr[128];
    struct evconnlistener* m_listener;
    std::map<struct bufferevent*, conn_node_base*> m_conn_nodes;
};

#endif
