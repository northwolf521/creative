#include "log.h"
#include "client_node_base.h"
#include "event2/bufferevent.h"

client_node_base::client_node_base(struct event_base* evbase) : _evbase(evbase) {
    m_port = 0;
    build_guid();
}

client_node_base::~client_node_base() {

}

void client_node_base::conn_succ_cb() {
    INFO("connect succ, host[%s:%d], nodeid[%d]",
         m_ip.c_str(), m_port, get_nodeid());
}

void client_node_base::conn_fail_cb() {
    INFO("connect fail, host[%s:%d], nodeid[%d]",
         m_ip.c_str(), m_port, get_nodeid());
}

void client_node_base::disconnect() {
    struct bufferevent* bev = get_bufferevent();
    if (bev) {
        bufferevent_free(bev);
        set_bufferevent(NULL);
    }
}

int client_node_base::connect() {
    if (!_evbase) {
        ERROR("_evbase is NULL");
        return -1;
    }

    if (is_connected()) {
        ERROR("duplicate connect, ip:%s, port:%d", m_ip.c_str(), m_port);
        return -1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(m_port);
    if (inet_pton(AF_INET, m_ip.c_str(), &sin.sin_addr) <= 0) {
        ERROR("inet_pton failed, ip:%s, port:%d", m_ip.c_str(), m_port);
        return -1;
    }

    struct bufferevent* bev = bufferevent_socket_new(_evbase, -1, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        ERROR("bufferevent_socket_new failed");
        return -1;
    }

    if (bufferevent_socket_connect(bev, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
        bufferevent_free(bev);
        return -1;
    }

    bufferevent_setcb(bev, client_node_base::conn_readcb, NULL, client_node_base::conn_eventcb, (void*)this);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
    set_bufferevent(bev);

    INFO("connect srv, host[%s:%d], bev[%p], nodeid[%d]",
         m_ip.c_str(), m_port, bev, get_nodeid());
    return 0;
}

void client_node_base::conn_readcb(struct bufferevent* bev, void* arg) {
    client_node_base* client_node = (client_node_base*)arg;
    int ret = client_node->do_recv();
    if (-1 == ret) {
        ERROR("recv data failed, close it, bev:%p, ret:%d, ip:%s, port:%d, nodebev:%p, nodeid[%u]",
              bev,
              ret,
              client_node->get_ip().c_str(),
              client_node->get_port(),
              client_node->get_bufferevent(),
              client_node->get_nodeid());

        client_node->set_bufferevent(NULL);
        bufferevent_free(bev);
    }
}

void client_node_base::conn_eventcb(struct bufferevent* bev, short events, void* arg) {
    client_node_base* client_node = (client_node_base*)arg;

    if (events & BEV_EVENT_ERROR) {
        INFO("client connect error, bev:%p, host:%s:%d, bev:%p, nodeid:%u, error(%d-%s)",
             bev,
             client_node->m_ip.c_str(),
             client_node->get_port(),
             client_node->get_bufferevent(),
             client_node->get_nodeid(),
             EVUTIL_SOCKET_ERROR(),
             evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
    }

    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        INFO("client connect fail, bev:%p, host:%s:%d, nodebev:%p, nodeid:%u, error(%d-%s)",
             bev,
             client_node->m_ip.c_str(),
             client_node->get_port(),
             client_node->get_bufferevent(),
             client_node->get_nodeid(),
             EVUTIL_SOCKET_ERROR(),
             evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));

        client_node->set_bufferevent(NULL);
        client_node->conn_fail_cb();
        bufferevent_free(bev);
        return;
    }

    if (events & BEV_EVENT_CONNECTED) {
        INFO("client connect succ, bev:%p, host:%s:%d, be:%p, nodeid:%u",
             bev,
             client_node->m_ip.c_str(),
             client_node->get_port(),
             client_node->get_bufferevent(),
             client_node->get_nodeid());

        int32_t fd = bufferevent_getfd(bev);
        if (-1 == evutil_make_socket_nonblocking(fd)) {
            ERROR("evutil_make_socket_nonblocking() failed, fd:%d", fd);
        }

        client_node->set_bufferevent(bev);
        client_node->conn_succ_cb();
        return;
    }
}
