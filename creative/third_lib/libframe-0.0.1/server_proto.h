#ifndef __SERVER_PROTO_H__
#define __SERVER_PROTO_H__

#include <stdint.h>
#include <errno.h>
#include "nanopb.h"

#define PROTO_MAGIC_VALUE1         0xCC
#define PROTO_MAGIC_VALUE2         0xEE
#define PROTO_DATA_SIZE_MAX        (1024 * 1024 * 1024)
#define BROADCAST_PLAYER_NUM_MAX   10000

#pragma pack(1)

struct ProtoHead {
    uint8_t  magic1; //标记1
    uint8_t  magic2; //标记2
    uint32_t len;    //长度
    uint32_t msgid;  //命令
    uint32_t seq;    //序号
    uint32_t crc;    //CRC32
};

struct ExternData {
    uint64_t playerid;  //角色id
    uint32_t sessid;    //会话id
    uint32_t userid;    //帐号id
    uint32_t regionid;  //大区ID
    uint32_t partid;    //小区ID
    uint32_t gameid;    //gameid
    uint32_t loginid;   //loginid
    void resetData() {
        playerid  = 0;
        sessid = 0;
        userid    = 0;
        regionid  = 0;
        partid    = 0;
        gameid    = 0;
        loginid   = 0;
    }
};

#pragma pack()

#endif
