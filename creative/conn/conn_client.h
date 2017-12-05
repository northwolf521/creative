#ifndef __CONN_CLIENT_H__
#define __CONN_CLIENT_H__

#include <stdint.h>
#include <string>
#include <math.h>
#include <algorithm>
#include "conn_node_base.h"
#include "noncopyable.h"

class conn_client : public conn_node_base, private noncopyable {
public:
    conn_client();
    virtual ~conn_client();

    void process_message(uint32_t msgid, const char* data, uint32_t size);
};

#endif
