#include "conn_node_mgr.h"

conn_node_mgr::conn_node_mgr() {

}

conn_node_mgr::~conn_node_mgr() {

}

conn_node_base* conn_node_mgr::random_node() {
    conn_node_base* target = NULL;

    int32_t num_of_node = _conn_node.size();
    if (num_of_node > 0)
        target = _conn_node[rand() % num_of_node];

    return target;
}

conn_node_base* conn_node_mgr::find_node(int32_t nodeid) {
    conn_node_base* target = NULL;

    int32_t  num_of_node = _conn_node.size();
    for (int32_t i = 0; i < num_of_node; i++) {
        auto p = _conn_node[i];
        if (!p || p->get_nodeid() != nodeid)
            continue;

        target = p;
        break;
    }

    return target;
}

int32_t conn_node_mgr::node_size() {
    return _conn_node.size();
}

int32_t conn_node_mgr::add_node(conn_node_base* node) {
    if (!node) {
        ERROR("node is NULL");
        return -1;
    }

    auto iter = std::find(_conn_node.begin(), _conn_node.end(), node);
    if (iter != _conn_node.end()) {
        ERROR("Repeating element, nodeid[%d]", node->get_nodeid());
        return -1;
    }

    _conn_node.push_back(node);
    return 0;
}

int32_t conn_node_mgr::del_node(conn_node_base* node) {
    if (!node) {
        ERROR("node is NULL");
        return -1;
    }

    auto iter = std::find(_conn_node.begin(), _conn_node.end(), node);
    if (iter == _conn_node.end()) {
        ERROR("Invalid element, nodeid[%d]", node->get_nodeid());
        return -1;
    }

    _conn_node.erase(iter);
    return 0;
}

int32_t conn_node_mgr::check_alive() {
    return 0;
}

int32_t conn_node_mgr::transfer_msg_ext(uint32_t msgid, const char* data, uint32_t size, const ExternData& ext) {
    int32_t  num_of_node = _conn_node.size();
    for (int32_t i = 0; i < num_of_node; i++) {
        auto p = _conn_node[i];
        if (NULL == p)
            continue;

        p->send_msg_ext(msgid, data, size, ext);
    }

    return 0;
}

int32_t conn_node_mgr::transfer_msg_ext(uint32_t msgid, const char* data, uint32_t size, const ExternData& ext, conn_node_base* expect) {
    int32_t  num_of_node = _conn_node.size();
    for (int32_t i = 0; i < num_of_node; i++) {
        auto p = _conn_node[i];
        if (NULL == p)
            continue;

        if (p == expect)
            continue;

        p->send_msg_ext(msgid, data, size, ext);
    }

    return 0;
}