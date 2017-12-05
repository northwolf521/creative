#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "log.h"
#include "noncopyable.h"
#include "singleton.h"
#include "server_node_base.h"
#include "conn_listen.h"
#include "redis_proxy.h"

class game_server : public server_node_base, private noncopyable {
public:
    game_server();
    virtual ~game_server();

    //创建资源
    int32_t init(int32_t argc, char** argv);
    //销毁资源
    int32_t final();
    //连接监听器
    conn_listen* listen_ptr();
    //服务回调
    void tick_proc();
    //redis管理
    redis_proxy* redis();
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
    conn_listen* _listen;
    redis_proxy* _redis;
};

#endif