#include "log.h"
#include "conn_node_base.h"
#include <arpa/inet.h>
#include <string.h>

conn_node_base::conn_node_base() {
    memset(&m_sin, 0, sizeof(m_sin));
    m_port = 0;
    m_link_srvid = 0;
    m_link_partid = 0;
    m_node_type = 0;
    build_guid();
}

conn_node_base::~conn_node_base() {

}

void conn_node_base::set_sockaddr(struct sockaddr_in sin) {
    m_sin = sin;
    const char* ip = inet_ntoa(m_sin.sin_addr);
    m_ip = ip ? ip : "";
    m_port = ntohs(m_sin.sin_port);
}
