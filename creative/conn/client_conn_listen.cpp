#include "log.h"
#include "client_conn_listen.h"
#include "conn_client.h"
#include <assert.h>
#include "nanopb.h"
#include "s2s.pb.h"

client_conn_listen::client_conn_listen(struct event_base* evbase) : listen_node_base(evbase) {

}

client_conn_listen::~client_conn_listen() {

}

bool client_conn_listen::accept_check(struct sockaddr_in addr) {
    INFO("client from %s:%d", inet_ntoa(addr.sin_addr), addr.sin_port);
    return true;
}

conn_node_base* client_conn_listen::alloc_conn_node() {
    auto node = new conn_client();
    if (node) {
        add_node(node);
    }

    return node;
}

void client_conn_listen::free_conn_node(conn_node_base* node) {
    if (node) {
        del_node(node);
        delete node;
        node = NULL;
    }
}
