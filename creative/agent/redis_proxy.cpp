#include "redis_proxy.h"

static void get_cb(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = (redisReply*)r;
    if (reply == NULL) {
        return;
    }

    DBG("@GET argv[%s]: %s", (char*)privdata, reply->str);
}

static void set_cb(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = (redisReply*)r;
    if (reply == NULL) {
        return;
    }

    DBG("@SET argv[%s]: %s", (char*)privdata, reply->str);
}


redis_proxy::redis_proxy(struct event_base* evbase) : async_redis_client(evbase) {

}

redis_proxy::~redis_proxy() {

}

void redis_proxy::test() {
    const char key[100] = "123456789";
    cmd(set_cb, (char*)"REQ_INDEX", "SET key %b", key, strlen(key));
    cmd(get_cb, (char*)"REQ_INDEX", "GET key");
}