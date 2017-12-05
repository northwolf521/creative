#ifndef __TCP_CONN_BASE_H__
#define __TCP_CONN_BASE_H__

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "event2/event.h"
#include "event2/event_compat.h"
#include "event2/event_struct.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "nanopb.h"
#include "server_proto.h"
#include "log.h"
#include "atomic.h"

//发送PROTOBUF协议数据
#define FAST_SEND_PROTOBUF(_conn, _msgid, _proto, _ext, _pack_func_pre)                                         \
    do {                                                                                                        \
        if ((_conn) == NULL) {                                                                                  \
            ERR_LOG(0, 0l, 0, "send msg failed, conn is null, msgid: %u, player id: %lu", _msgid, (_ext)->player_id);       \
            break;                                                                                              \
        }                                                                                                       \
        size_t _size = _pack_func_pre##__get_packed_size(_proto);                                               \
        char* _sendBuff = NULL;                                                                                 \
        if (_size > 0) {                                                                                        \
            _sendBuff = (char*)malloc(_size + 1);                                                               \
            if (_sendBuff == NULL) {                                                                            \
                ERR_LOG(0, 0l, 0, "alloc memory failed when pack protobuf, msgid: %u, size: %u, player id: %lu", _msgid, (uint32_t)_size, (_ext)->player_id);   \
                break;                                                                                          \
            }                                                                                                   \
            _pack_func_pre##__pack(_proto, (uint8_t*)_sendBuff);                                                \
        }                                                                                                       \
        bool _sendRet = (_conn)->send_msg_ext(_msgid, _sendBuff, _size, *(_ext));                               \
        if (!_sendRet) {                                                                                        \
            ERR_LOG(0, 0l, 0, "send msg failed, msgid: %u, size: %u, player id: %lu", _msgid, (uint32_t)_size, (_ext)->player_id);  \
        }                                                                                                       \
        if (_sendBuff) {                                                                                        \
            free(_sendBuff);                                                                                    \
            _sendBuff = NULL;                                                                                   \
        }                                                                                                       \
    } while(0)

//发送NANOPB协议数据
#define FAST_SEND_NANOPB(_conn, _msgid, _type, _proto, _ext)                                                    \
    do {                                                                                                        \
        if ((_conn) == NULL) {                                                                                  \
            ERR_LOG(0, 0l, 0, "send msg failed, conn is null, msgid: %u, player id: %lu",                       \
                _msgid, (_ext)->player_id);                                                                     \
            break;                                                                                              \
        }                                                                                                       \
        char* _sendBuff = (char*)malloc(NANOPB_SIZE(_type) + 1);                                                \
        if (_sendBuff == NULL) {                                                                                \
            ERR_LOG(0, 0l, 0, "alloc memory failed when pack nanopb, msgid: %u, size: %u, player id: %lu",      \
                _msgid, (uint32_t)NANOPB_SIZE(_type), (_ext)->player_id);                                       \
            break;                                                                                              \
        }                                                                                                       \
        pb_ostream_t _stream = pb_ostream_from_buffer((uint8_t*)_sendBuff, (size_t)NANOPB_SIZE(_type));         \
        if (!pb_encode(&_stream, NANOPB_FIELDS(_type), &_proto)) {                                              \
            ERR_LOG(0, 0l, 0, "Encoding failed: %s, msgid: %u, size: %u, player id: %lu",                       \
                PB_GET_ERROR(&_stream), _msgid, (uint32_t)NANOPB_SIZE(_type), (_ext)->player_id);               \
            free(_sendBuff);                                                                                    \
            _sendBuff = NULL;                                                                                   \
            break;                                                                                              \
        }                                                                                                       \
        bool _sendRet = (_conn)->send_msg_ext(_msgid, _sendBuff, _stream.bytes_written, *(_ext));               \
        if (!_sendRet) {                                                                                        \
            ERR_LOG(0, 0l, 0, "send msg failed, msgid: %u, size: %u, player id: %lu",                           \
                _msgid, (uint32_t)NANOPB_SIZE(_type), (_ext)->player_id);                                       \
        }                                                                                                       \
        if (_sendBuff) {                                                                                        \
            free(_sendBuff);                                                                                    \
            _sendBuff = NULL;                                                                                   \
        }                                                                                                       \
    } while (0)

//使用内部协议数据 发送到其它服务器
#define FAST_SEND_ROWDATA(_conn, _msgid, _data, _size, _ext)                                                    \
    do {                                                                                                        \
        if ((_conn) == NULL) {                                                                                  \
            ERR_LOG(0, 0l, 0, "send msg failed, conn is null, msgid: %u, player id: %lu", _msgid, (_ext)->player_id);       \
            break;                                                                                              \
        }                                                                                                       \
        bool _sendRet = (_conn)->send_msg_ext(_msgid, (const char*)_data, _size, *(_ext));                      \
        if (!_sendRet) {                                                                                        \
            ERR_LOG(0, 0l, 0, "send msg failed, msgid: %u, size: %u, player id: %lu", _msgid, (uint32_t)_size, (_ext)->player_id);  \
        }                                                                                                       \
    } while(0)


//发送广播数据
#define FAST_BROADCAST_PROTOBUF(_conn, _player_ids, _msgid, _proto, _ext, _pack_func_pre)                       \
    do {                                                                                                        \
        if ((_conn) == NULL) {                                                                                  \
            ERR_LOG(0, 0l, 0, "broadcast msg failed, conn is null, msgid: %u, player id: %lu", _msgid, (_ext)->player_id);  \
            break;                                                                                              \
        }                                                                                                       \
        size_t _size = _pack_func_pre##__get_packed_size(_proto);                                               \
        char* _sendBuff = NULL;                                                                                 \
        if (_size > 0) {                                                                                        \
            _sendBuff = (char*)malloc(_size + 1);                                                               \
            if (_sendBuff == NULL) {                                                                            \
                ERR_LOG(0, 0l, 0, "alloc memory failed when pack protobuf, msgid: %u, size: %u, player id: %lu", _msgid, (uint32_t)_size, (_ext)->player_id);   \
                break;                                                                                          \
            }                                                                                                   \
            _pack_func_pre##__pack(_proto, (uint8_t*)_sendBuff);                                                \
        }                                                                                                       \
        bool _sendRet = (_conn)->send_broadcast_msg(_player_ids, _msgid, _sendBuff, _size, *(_ext));            \
        if (!_sendRet) {                                                                                        \
            ERR_LOG(0, 0l, 0, "broadcast msg failed, msgid: %u, size: %u, player id: %lu", _msgid, (uint32_t)_size, (_ext)->player_id); \
        }                                                                                                       \
        if (_sendBuff) {                                                                                        \
            free(_sendBuff);                                                                                    \
            _sendBuff = NULL;                                                                                   \
        }                                                                                                       \
    } while (0)

#define FAST_BROADCAST_ROWDATA(_conn, _player_ids, _msgid, _data, _size, _ext)                                  \
    do {                                                                                                        \
        if ((_conn) == NULL) {                                                                                  \
            ERR_LOG(0, 0l, 0, "broadcast msg failed, conn is null, msgid: %u, player id: %lu", _msgid, (_ext)->player_id);  \
            break;                                                                                              \
        }                                                                                                       \
        bool _sendRet = (_conn)->send_broadcast_msg(_player_ids, _msgid, (const char*)_data, _size, *(_ext));   \
        if (!_sendRet) {                                                                                        \
            ERR_LOG(0, 0l, 0, "broadcast msg failed, msgid: %u, size: %u, player id: %lu", _msgid, (uint32_t)_size, (_ext)->player_id); \
        }                                                                                                       \
    } while(0)

class tcp_conn_base {
public:
    tcp_conn_base();
    virtual ~tcp_conn_base();

    //检验码表
    static uint32_t s_crc32_table[256];
    //检验算法
    static uint32_t get_crc32(const char* data, uint32_t size);

    //连接数据处理
    virtual void process_message(uint32_t msgid, const char* data, uint32_t size) = 0;

    //设置事件缓存对象
    void set_bufferevent(struct bufferevent* buffev);
    //获取事件缓存对象
    struct bufferevent* get_bufferevent();
    //接收数据
    int32_t do_recv();
    //发送玩家消息
    bool send_msg(uint32_t msgid, const char* data, uint32_t size);
    //发送服务消息
    bool send_msg_ext(uint32_t msgid, const char* data, uint32_t size, const ExternData& ext);
    //广播消息
    bool send_broadcast_msg(std::vector<uint64_t>* player_ids, uint32_t msgid, const char* data, uint32_t size, const ExternData& ext);
    //设置连接标识
    void set_linkid(int32_t linkid);
    //获取连接标识
    int32_t get_linkid();
    //与其连接的主机信息
    int32_t set_link_host(const char* host);
    //与其连接的地址
    char* get_link_addr();
    //与其连接的端口
    int32_t get_link_port();
    //获取获取标识码
    int32_t get_nodeid();
    //设置最后访问时间
    void set_last_access_time(int64_t access_time);
    //获取最后访问时间
    int64_t get_last_access_time();

private:
    //数据缓存大小检查
    void check_recv_buf_size(int size);

protected:
    //生成唯一标识
    uint32_t build_guid();

private:
    struct bufferevent* m_bufferevent;
    char*   m_recvBuf;
    int32_t m_recvBufSize;
    int32_t m_recvSize;
    int32_t m_nodeid;
    int32_t m_linkid;
    int32_t m_linkport;
    char    m_linkaddr[128];
    int64_t m_last_access_time;
};

#endif
