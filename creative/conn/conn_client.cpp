#include "conn_client.h"
#include "log.h"
#include "s2s.pb.h"
#include "singleton.h"
#include "conn_server.h"

conn_client::conn_client() {

}

conn_client::~conn_client() {

}

void conn_client::process_message(uint32_t msgid, const char* data, uint32_t size) {
    if (size < sizeof(ExternData)) {
        ERROR("msg data error, no extern data, msgid:%u, size:%u", msgid, size);
        return;
    }

    uint32_t bodysize = size - sizeof(ExternData);
    ExternData* ext = (ExternData*)(data + bodysize);
    singleton<conn_server>::instance()->client_listen_ptr()->transfer_msg_ext(msgid, data, bodysize, *ext, this);
}
