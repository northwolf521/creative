#ifndef __SERVER_CONN_LISTEN_H__
#define __SERVER_CONN_LISTEN_H__

#include "listen_node_base.h"
#include "noncopyable.h"
#include "conn_node_mgr.h"
#include "singleton.h"

class server_conn_listen : public listen_node_base, public conn_node_mgr, private noncopyable {
public:
    server_conn_listen(struct event_base* evbase);
    virtual ~server_conn_listen();

    //连接检查
    virtual bool accept_check(struct sockaddr_in addr);
    //分配节点
    virtual conn_node_base* alloc_conn_node();
    //释放节点
    virtual void free_conn_node(conn_node_base* node);
};

#endif
