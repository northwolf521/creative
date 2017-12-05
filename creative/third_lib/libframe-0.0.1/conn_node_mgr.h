#ifndef __CONN_NODE_MANAGER_H__
#define __CONN_NODE_MANAGER_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "singleton.h"
#include "conn_node_base.h"
#include "lock.h"
#include "noncopyable.h"

class conn_node_mgr {
    typedef std::vector<conn_node_base*> conn_node_list_t;

public:
    conn_node_mgr();
    virtual ~conn_node_mgr();

    conn_node_base* random_node();
    conn_node_base* find_node(int32_t nodeid);
    int32_t node_size();
    int32_t add_node(conn_node_base* node);
    int32_t del_node(conn_node_base* node);
    int32_t check_alive();
    int32_t transfer_msg_ext(uint32_t msgid, const char* data, uint32_t size, const ExternData& ext);
    int32_t transfer_msg_ext(uint32_t msgid, const char* data, uint32_t size, const ExternData& ext, conn_node_base* expect);

protected:
    conn_node_list_t _conn_node;
};

#endif
