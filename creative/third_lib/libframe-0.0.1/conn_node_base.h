#ifndef __CONN_NODE_BASE_H__
#define __CONN_NODE_BASE_H__

#include <netinet/in.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "tcp_conn_base.h"

class conn_node_base : public tcp_conn_base {
public:
    conn_node_base();
    virtual ~conn_node_base();

    void set_sockaddr(struct sockaddr_in sin);
    struct sockaddr_in get_sockaddr() {
        return m_sin;
    }
    std::string get_ip() {
        return m_ip;
    }
    uint16_t get_port() {
        return m_port;
    }
    uint32_t get_link_srvid() {
        return m_link_srvid;
    }
    void set_link_srvid(uint32_t srvid) {
        this->m_link_srvid = srvid;
    }
    uint32_t get_link_partid() {
        return m_link_partid;
    }
    void set_link_partid(uint32_t partid) {
        this->m_link_partid = partid;
    }
    uint32_t get_type() {
        return this->m_node_type;
    }
    void set_type(uint32_t type) {
        this->m_node_type = type;
    }

private:
    struct sockaddr_in m_sin;
    std::string        m_ip;
    uint16_t           m_port;
    uint32_t           m_link_srvid;
    uint32_t           m_link_partid;
    uint32_t           m_node_type;
};

#endif
