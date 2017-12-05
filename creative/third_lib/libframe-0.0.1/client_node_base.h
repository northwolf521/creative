#ifndef __CLIENT_NODE_BASE_H__
#define __CLIENT_NODE_BASE_H__

#include <stdint.h>
#include <string>
#include "tcp_conn_base.h"

class client_node_base : public tcp_conn_base {
public:
    client_node_base(struct event_base* evbase);
    virtual ~client_node_base();

    virtual void conn_succ_cb();

    virtual void conn_fail_cb();

    bool is_connected() {
        return get_bufferevent() != NULL;
    }

    void set_ip(const char* ip) {
        m_ip = ip ? ip : "";
    }

    std::string get_ip() {
        return m_ip;
    }

    void set_port(uint16_t port) {
        m_port = port;
    }

    uint16_t get_port() {
        return m_port;
    }

    void set_type(int32_t type) {
        m_type = type;
    }

    int32_t get_type() {
        return m_type;
    }

    void disconnect();

    int connect();

private:
    static void conn_readcb(struct bufferevent* bev, void* arg);

    static void conn_eventcb(struct bufferevent* bev, short events, void* arg);

protected:
    struct event_base* _evbase;

private:
    std::string m_ip;
    uint16_t m_port;
    int32_t m_type;
};

#endif
