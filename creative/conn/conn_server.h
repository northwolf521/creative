#ifndef __CONN_SERVER_H__
#define __CONN_SERVER_H__

#include "log.h"
#include "noncopyable.h"
#include "singleton.h"
#include "server_node_base.h"
#include "client_conn_listen.h"
#include "server_conn_listen.h"

class conn_server : public server_node_base, private noncopyable {
public:
    conn_server();
    virtual ~conn_server();

    //创建资源
    int32_t init(int32_t argc, char** argv);
    //销毁资源
    int32_t final();
    //前端连接监听器
    client_conn_listen* client_listen_ptr();
    //后端连接监听器
    server_conn_listen* server_listen_ptr();
    //服务回调
    void tick_proc();
    //重载资源
    void reload();

public:
    //usr1信号
    void signal_usr1();
    //usr2信号
    void signal_usr2();
    //int信号
    void signal_int();

private:
    client_conn_listen* _client_listen;
    server_conn_listen* _server_listen;
};

#endif
