#include "tcp_conn_base.h"
#include "event2/bufferevent.h"

uint32_t tcp_conn_base::s_crc32_table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B, 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

uint32_t tcp_conn_base::get_crc32(const char* data, uint32_t size) {
    if (size == 0) {
        return 0;
    }

    uint8_t tmp = 0;
    uint32_t crc = 0xFFFFFFFF;
    for (uint32_t i = 0; i < size; ++i) {
        tmp =  (crc & 0xFF);
        tmp ^= data[i];
        crc =  (crc >> 8) ^ s_crc32_table[tmp];
    }

    crc ^= 0xFFFFFFFF;
    return crc;
}

tcp_conn_base::tcp_conn_base() {
    m_bufferevent = NULL;
    m_recvBuf = NULL;
    m_recvBufSize = 0;
    m_recvSize = 0;
    m_nodeid = 0;
    m_linkid = 0;
    m_linkport = 0;
    m_last_access_time = 0;
    memset(m_linkaddr, 0, sizeof(m_linkaddr));
}

tcp_conn_base::~tcp_conn_base() {
    if (m_recvBuf) {
        free(m_recvBuf);
        m_recvBuf = NULL;
    }
}

void tcp_conn_base::set_bufferevent(struct bufferevent* buffev) {
    m_bufferevent = buffev;
    m_recvSize = 0;
}

struct bufferevent* tcp_conn_base::get_bufferevent() {
    return m_bufferevent;
}

void tcp_conn_base::check_recv_buf_size(int size) {
    if (size > m_recvBufSize) {
        if (m_recvBuf)
            m_recvBuf = (char*)realloc(m_recvBuf, size);
        else
            m_recvBuf = (char*)malloc(size);

        m_recvBufSize = size;
        assert(m_recvBuf);
    }
}

int32_t tcp_conn_base::do_recv() {
    assert(m_recvSize >= 0 && m_recvSize <= m_recvBufSize);

    if (!m_bufferevent) {
        ERROR("bufferevent is null");
        return -1;
    }

    check_recv_buf_size(64 * 1024);

    if (m_recvSize >= m_recvBufSize) {
        ERROR("recv buf is full");
        return -1;
    }

    for (;;) {
        size_t ret = bufferevent_read(m_bufferevent, m_recvBuf + m_recvSize, m_recvBufSize - m_recvSize);
        if (ret == 0) {
            return 0;
        }

        m_recvSize += ret;

        int32_t  begin_pos = 0;
        uint32_t headSize = sizeof(ProtoHead);
        for (;;) {
            while (m_recvSize - begin_pos >= 2 && (uint8_t)m_recvBuf[begin_pos] != (uint8_t)PROTO_MAGIC_VALUE1
                    && (uint8_t)m_recvBuf[begin_pos + 1] != (uint8_t)PROTO_MAGIC_VALUE2) {
                ++begin_pos;
            }

            if (m_recvSize - begin_pos < (int)headSize) {
                break;
            }

            ProtoHead* head = (ProtoHead*)(m_recvBuf + begin_pos);
            uint32_t msgid = ntohl(head->msgid);
            uint32_t len   = ntohl(head->len);
            if (len > PROTO_DATA_SIZE_MAX) {
                ERROR("proto data too large, msgid:%u, len:%u", msgid, len);
                m_recvSize = 0;
                return -1;
            }

            if (len < headSize - 2) {
                len = headSize - 2;
            }

            check_recv_buf_size(((len + 2) / 1024 + 1) * 1024);

            if (m_recvSize - begin_pos - 2 < (int32_t)len) {
                break;
            }

            uint32_t headCrc32 = ntohl(head->crc);
            char*    bodyData  = m_recvBuf + begin_pos + headSize;
            uint32_t bodySize  = len - headSize + 2;

            //因为旧协议写死了3333，为了兼容旧的，只有crc不为3333时才检验
            bool valid = true;
            if (headCrc32 != 3333) {
                uint32_t calcCrc32 = get_crc32(bodyData, bodySize);
                if (headCrc32 != calcCrc32) {
                    valid = false;
                    ERROR("valid crc failed, msgid:%u, calcCrc32[%u]", msgid, calcCrc32);
                }
            }

            if (valid) {
                process_message(msgid, m_recvBuf + begin_pos + headSize, len - headSize + 2);
            }

            begin_pos += 2;
            begin_pos += len;
        }

        if (m_recvSize > begin_pos) {
            memmove(m_recvBuf, m_recvBuf + begin_pos, m_recvSize - begin_pos);
            m_recvSize -= begin_pos;
        }
        else {
            m_recvSize = 0;
        }
    }

    return 0;
}

bool tcp_conn_base::send_msg(uint32_t msgid, const char* data, uint32_t size) {
    if (!m_bufferevent) {
        ERROR("bufferevent is null, msgid:%u", msgid);
        return false;
    }

    if (size > PROTO_DATA_SIZE_MAX) {
        ERROR("proto data too large, msgid:%u, size:%u", msgid, size);
        return false;
    }

    int32_t headSize = sizeof(ProtoHead);

    ProtoHead head;
    head.magic1 = PROTO_MAGIC_VALUE1;
    head.magic2 = PROTO_MAGIC_VALUE2;
    head.len    = htonl(headSize - 2 + size);
    head.msgid  = htonl(msgid);
    head.seq    = 0;
    head.crc    = htonl(get_crc32(data, size));

    char* bodyBuff = (char*)malloc(headSize + size);
    if (!bodyBuff) {
        ERROR("malloc buff failed, bodySize[%d]", headSize + size);
        return false;
    }

    memcpy(bodyBuff, &head, headSize);
    memcpy(bodyBuff + headSize, data, size);
    if (bufferevent_write(m_bufferevent, bodyBuff, headSize + size) != 0) {
        ERROR("send message %d failed", msgid);
        free(bodyBuff);
        return false;
    }

    DBG("send message %d success, size[%d]", msgid, headSize + size);

    free(bodyBuff);
    return true;
}

bool tcp_conn_base::send_msg_ext(uint32_t msgid, const char* data, uint32_t size, const ExternData& ext) {
    if (!m_bufferevent) {
        ERROR("bufferevent is null, msgid:%u", msgid);
        return false;
    }

    if (size > PROTO_DATA_SIZE_MAX) {
        ERROR("proto data too large, msgid:%u, size:%u, playerid:%lu", msgid, size, ext.playerid);
        return false;
    }

    uint32_t headSize = sizeof(ProtoHead);
    uint32_t bodySize = size + sizeof(ExternData);
    char* bodyBuff = (char*)malloc(headSize + bodySize);
    if (!bodyBuff) {
        ERROR("malloc body buff failed, msgid:%u, bodySize:%u", msgid, bodySize);
        return false;
    }

    memcpy(bodyBuff + headSize, data, size);
    memcpy(bodyBuff + headSize + size, &ext, sizeof(ExternData));

    ProtoHead head;
    memset(&head, 0, sizeof(head));
    head.magic1 = PROTO_MAGIC_VALUE1;
    head.magic2 = PROTO_MAGIC_VALUE2;
    head.len    = htonl(headSize - 2 + bodySize);
    head.msgid  = htonl(msgid);
    head.seq    = 0;
    head.crc    = 0;
    // head.crc    = htonl(get_crc32(bodyBuff + headSize, bodySize));
    memcpy(bodyBuff, &head, headSize);
    if (bufferevent_write(m_bufferevent, bodyBuff, bodySize + headSize) != 0) {
        ERROR("forward message %d session %d failed", msgid, ext.sessid);
        free(bodyBuff);
        return false;
    }

    DBG("forward message %d session %d success", msgid, ext.sessid);

    free(bodyBuff);
    return true;
}

bool tcp_conn_base::send_broadcast_msg(std::vector<uint64_t>* playerids, uint32_t msgid, const char* data, uint32_t size, const ExternData& ext) {
    if (!m_bufferevent) {
        ERROR("bufferevent is null, msgid:%u", msgid);
        return false;
    }

    if (size > PROTO_DATA_SIZE_MAX) {
        ERROR("proto data too large, msgid:%u, size:%u, playerid:%lu", msgid, size, ext.playerid);
        return false;
    }

    if (playerids && playerids->size() > BROADCAST_PLAYER_NUM_MAX) {
        ERROR("broadcast player too many, player num:%lu, msgid:%u, playerid:%lu",
              playerids->size(), msgid, ext.playerid);
        return false;
    }

    uint32_t headSize  = sizeof(ProtoHead);
    uint16_t playerNum = playerids ? playerids->size() : 0;
    uint32_t bodySize  = sizeof(uint16_t) + playerNum * sizeof(uint64_t) + sizeof(uint32_t) + size + sizeof(ExternData);
    char* bodyBuff = (char*)malloc(headSize + bodySize);
    if (!bodyBuff) {
        ERROR("malloc body buff failed, msgid:%u, size:%u, playerid:%lu, bodySize:%u",
              msgid, size, ext.playerid, bodySize);
        return false;
    }

    memcpy(bodyBuff + headSize, &playerNum, sizeof(uint16_t));

    if (playerNum > 0)
        memcpy(bodyBuff + headSize + sizeof(uint16_t), &(*playerids)[0], playerNum * sizeof(uint64_t));

    memcpy(bodyBuff + headSize + sizeof(uint16_t) + playerNum * sizeof(uint64_t), &msgid, sizeof(uint32_t));

    if (size > 0)
        memcpy(bodyBuff + headSize + sizeof(uint16_t) + playerNum * sizeof(uint64_t) + sizeof(uint32_t), data, size);

    memcpy(bodyBuff + headSize + sizeof(uint16_t) + playerNum * sizeof(uint64_t) + sizeof(uint32_t) + size, &ext, sizeof(ExternData));

    ProtoHead head;
    head.magic1 = PROTO_MAGIC_VALUE1;
    head.magic2 = PROTO_MAGIC_VALUE2;
    head.len    = htonl(headSize - 2 + bodySize);
    head.msgid  = htonl(30000);
    head.seq    = 0;
    head.crc    = htonl(get_crc32(bodyBuff + headSize, bodySize));
    memcpy(bodyBuff, &head, headSize);
    if (bufferevent_write(m_bufferevent, bodyBuff, bodySize + headSize) != 0) {
        ERROR("broadcast message %d failed", head.msgid);
        free(bodyBuff);
        return false;
    }

    DBG("broadcast message %d success", head.msgid);

    free(bodyBuff);
    return true;
}

void tcp_conn_base::set_linkid(int32_t linkid) {
    this->m_linkid = linkid;
}

int32_t tcp_conn_base::get_linkid() {
    return this->m_linkid;
}

int32_t tcp_conn_base::set_link_host(const char* host) {
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

    int32_t addrlen = sizeof(this->m_linkaddr);
    memset(this->m_linkaddr, 0, addrlen);
    strncpy(this->m_linkaddr, v[0].c_str(), addrlen - 1);
    this->m_linkport = atoi(v[1].c_str());
    return 0;
}

char* tcp_conn_base::get_link_addr() {
    return &(this->m_linkaddr[0]);
}

int32_t tcp_conn_base::get_link_port() {
    return this->m_linkport;
}

int32_t tcp_conn_base::get_nodeid() {
    return this->m_nodeid;
}

void tcp_conn_base::set_last_access_time(int64_t access_time) {
    this->m_last_access_time = access_time;
}

int64_t tcp_conn_base::get_last_access_time() {
    return this->m_last_access_time;
}

uint32_t tcp_conn_base::build_guid() {
    static uint32_t guid = 0;
    guid = ATOMIC_INC(&guid);
    if (guid == 0)
        guid = ATOMIC_INC(&guid);

    this->m_nodeid = guid;
    return this->m_nodeid;
}
