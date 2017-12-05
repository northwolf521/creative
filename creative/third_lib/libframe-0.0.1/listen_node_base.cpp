#include "log.h"
#include "listen_node_base.h"
#include <arpa/inet.h>
#include <string.h>
#include <event2/bufferevent_struct.h>
#include "atomic.h"

listen_node_base::listen_node_base(struct event_base* evbase) : _evbase(evbase) {
    m_port = 0;
    m_listener = NULL;
    memset(m_addr, 0, sizeof(m_addr));
}

listen_node_base::~listen_node_base() {
    close();
}

bool listen_node_base::accept_check(struct sockaddr_in addr) {
    return true;
}

void listen_node_base::close() {
    std::map<struct bufferevent*, conn_node_base*>::iterator itr;
    for (itr = m_conn_nodes.begin(); itr != m_conn_nodes.end(); ++itr) {
        free_conn_node(itr->second);
        bufferevent_free(itr->first);
    }

    m_conn_nodes.clear();

    if (m_listener) {
        evconnlistener_free(m_listener);
        m_listener = NULL;
    }
}

int listen_node_base::listen() {
    if (!_evbase) {
        ERROR("_evbase is NULL");
        return -1;
    }

    if (m_listener) {
        ERROR("duplicate listen, port:%d", m_port);
        return -1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(m_port);
    m_listener = evconnlistener_new_bind(_evbase,
                                         listen_node_base::listener_cb,
                                         (void*)this,
                                         LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                         -1,
                                         (struct sockaddr*)&sin,
                                         sizeof(sin));
    if (!m_listener) {
        ERROR("evconnlistener_new_bind failed, errno:%d, port:%d",
              EVUTIL_SOCKET_ERROR(), m_port);
        return -1;
    }

    INFO("listen ok, port:%d, listener:%p", m_port, m_listener);
    return 0;
}

void listen_node_base::listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int salen, void* arg) {
    listen_node_base* listen_node = (listen_node_base*)arg;

    struct sockaddr_in sin;
    memcpy(&sin, sa, salen);
    const char* ip = inet_ntoa(sin.sin_addr);
    uint16_t port = ntohs(sin.sin_port);
    if (!listen_node->accept_check(sin)) {
        ERROR("accept_check failed, refuse it, listener:%p, port:%d, client: [%s:%d]",
              listen_node->m_listener, listen_node->m_port, ip,  port);
        evutil_closesocket(fd);
        return;
    }

    struct bufferevent* bev = bufferevent_socket_new(listen_node->_evbase, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        ERROR("bufferevent_socket_new failed, listener:%p, port:%d, client: [%s:%d]",
              listen_node->m_listener, listen_node->m_port, ip, port);
        evutil_closesocket(fd);
        return;
    }

    if (listen_node->m_conn_nodes.find(bev) != listen_node->m_conn_nodes.end()) {
        ERROR("bev has exist when accept client, listener:%p, port:%d, client: [%s:%d], bev:%p",
              listen_node->m_listener, listen_node->m_port, ip, port, bev);
        bufferevent_free(bev);
        return;
    }

    if (-1 == evutil_make_socket_nonblocking(fd)) {
        ERROR("evutil_make_socket_nonblocking() failed, listener:%p, fd:%d", listen_node->m_listener, fd);
    }

    conn_node_base* conn_node = listen_node->alloc_conn_node();
    if (!conn_node) {
        ERROR("alloc_conn_node failed, refuse it, listener:%p, port:%d, client: [%s:%d]",
              listen_node->m_listener, listen_node->m_port, ip, port);
        bufferevent_free(bev);
        return;
    }

    bufferevent_setcb(bev, listen_node_base::conn_readcb, NULL, listen_node_base::conn_eventcb, (void*)listen_node);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    conn_node->set_bufferevent(bev);
    conn_node->set_sockaddr(sin);
    listen_node->m_conn_nodes[bev] = conn_node;

    INFO("accept client, listener:%p, port:%d, client:[%s:%d], bev:%p, node:[%s:%d:%p], guid[%d]",
         listen_node->m_listener,
         listen_node->m_port,
         ip,
         port,
         bev,
         conn_node->get_ip().c_str(),
         conn_node->get_port(),
         conn_node->get_bufferevent(),
         conn_node->get_nodeid());
}

void listen_node_base::conn_readcb(struct bufferevent* bev, void* arg) {
    if (!bev) {
        return;
    }

    listen_node_base* listen_node = (listen_node_base*)arg;
    std::map<struct bufferevent*, conn_node_base*>::iterator itr = listen_node->m_conn_nodes.find(bev);
    if (itr == listen_node->m_conn_nodes.end()) {
        ERROR("find conn node failed when readcb, listener:%p, port:%d, bev:%p",
              listen_node->m_listener,
              listen_node->m_port,
              bev);
        return;
    }

    conn_node_base* conn_node = itr->second;
    int ret = conn_node->do_recv();
    if (ret != 0) {
        ERROR("recv client failed, close it, ret:%d, listener:%p, port:%d, bev:%p, node:[%s:%d:%p]",
              ret,
              listen_node->m_listener,
              listen_node->m_port, bev,
              conn_node->get_ip().c_str(),
              conn_node->get_port(),
              conn_node->get_bufferevent());

        listen_node->free_conn_node(conn_node);
        listen_node->m_conn_nodes.erase(itr);
        bufferevent_free(bev);
    }
}

void listen_node_base::conn_eventcb(struct bufferevent* bev, short events, void* arg) {
    if (!bev) {
        return;
    }

    listen_node_base* listen_node = (listen_node_base*)arg;
    std::map<struct bufferevent*, conn_node_base*>::iterator itr = listen_node->m_conn_nodes.find(bev);
    if (itr == listen_node->m_conn_nodes.end()) {
        ERROR("find conn node failed when eventcb, listener:%p, port:%d, bev:%p",
              listen_node->m_listener,
              listen_node->m_port, bev);
        return;
    }

    conn_node_base* conn_node = itr->second;

    DBG("conn node event, bev:%p, events: 0x%x, listener:%p, port:%d, node:[%s:%d:%p]",
        bev,
        events,
        listen_node->m_listener,
        listen_node->m_port,
        conn_node->get_ip().c_str(),
        conn_node->get_port(),
        conn_node->get_bufferevent());

    if (events & BEV_EVENT_ERROR) {
        DBG("conn node error, bev:%p, listener:%p, port:%d, node:[%s:%d:%p]",
            bev,
            listen_node->m_listener,
            listen_node->m_port,
            conn_node->get_ip().c_str(),
            conn_node->get_port(),
            conn_node->get_bufferevent());
    }

    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        DBG("close conn node, bev:%p, listener:%p, port:%d, node:[%s:%d:%p]",
            bev,
            listen_node->m_listener,
            listen_node->m_port,
            conn_node->get_ip().c_str(),
            conn_node->get_port(),
            conn_node->get_bufferevent());

        listen_node->free_conn_node(conn_node);
        listen_node->m_conn_nodes.erase(itr);
        bufferevent_free(bev);
    }
}

void listen_node_base::close_conn_node(conn_node_base* conn_node) {
    if (!conn_node) {
        return;
    }

    struct bufferevent* bev = conn_node->get_bufferevent();
    if (!bev) {
        ERROR("bufferevent is null when close node, node:[%s:%d:%p]",
              conn_node->get_ip().c_str(),
              conn_node->get_port(),
              conn_node->get_bufferevent());
        return;
    }

    listen_node_base* listen_node = (listen_node_base*)bev->cbarg;
    if (!listen_node) {
        ERROR("listen_node is null when close node, node:[%s:%d:%p]",
              conn_node->get_ip().c_str(),
              conn_node->get_port(),
              conn_node->get_bufferevent());
        return;
    }

    std::map<struct bufferevent*, conn_node_base*>::iterator itr = listen_node->m_conn_nodes.find(bev);
    if (itr == listen_node->m_conn_nodes.end()) {
        ERROR("find conn node failed when close node, listener:%p, port:%d, node:[%s:%d:%p]",
              listen_node->m_listener,
              listen_node->m_port,
              conn_node->get_ip().c_str(),
              conn_node->get_port(),
              conn_node->get_bufferevent());
        return;
    }

    if (conn_node != itr->second) {
        ERROR("conn node mismatch when close node, listener:%p, port:%d, req_node:[%s:%d:%p], has_node:[%s:%d:%p]",
              listen_node->m_listener,
              listen_node->m_port,
              conn_node->get_ip().c_str(),
              conn_node->get_port(),
              conn_node->get_bufferevent(),
              itr->second->get_ip().c_str(),
              itr->second->get_port(),
              itr->second->get_bufferevent());
        return;
    }

    DBG("close conn node, listener:%p, port:%d, node:[%s:%d:%p]",
        listen_node->m_listener,
        listen_node->m_port,
        conn_node->get_ip().c_str(),
        conn_node->get_port(),
        conn_node->get_bufferevent());

    listen_node->free_conn_node(conn_node);
    listen_node->m_conn_nodes.erase(itr);

    bufferevent_free(bev);
}

void listen_node_base::set_port(uint16_t port) {
    m_port = port;
}

uint16_t listen_node_base::get_port() {
    return m_port;
}

int32_t listen_node_base::set_hostinfo(const char* host) {
    if (!host) {
        ERROR("host is null");
        return -1;
    }

    char chost[128] = {'\0'};
    int32_t len = sizeof(chost);
    strncpy(chost, host, len - 1);
    chost[len - 1] = '\0';

    std::vector<std::string> v;
    char* p = strtok (chost, ":");
    while (p) {
        v.push_back(p);
        p = strtok(NULL, ":");
    }

    if (2 != v.size()) {
        ERROR("Parse error, [%s]", host);
        return -1;
    }

    int32_t addrlen = sizeof(this->m_addr);
    memset(this->m_addr, 0, addrlen);
    strncpy(this->m_addr, v[0].c_str(), addrlen - 1);
    this->m_port = atoi(v[1].c_str());
    return 0;
}