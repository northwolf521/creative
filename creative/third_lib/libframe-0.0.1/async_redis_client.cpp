#include "async_redis_client.h"
#include "adapters/libevent.h"

static void redis_connect_cb(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        ERROR("redis connect errstr: %s", c->errstr);
        return;
    }

    async_redis_client* pridata = (async_redis_client*)(c->data);
    if (!pridata) {
        ERROR("redis_connect_cb(): pridata is NULL");
        return;
    }

    pridata->cb_connected();
}

static void redis_disconnect_cb(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        ERROR("redis discconnect errstr: %s", c->errstr);
        return;
    }

    async_redis_client* pridata = (async_redis_client*)(c->data);
    if (!pridata) {
        ERROR("redis_disconnect_cb(): pridata is NULL");
        return;
    }

    pridata->cb_disconnected();
}

async_redis_client::async_redis_client(struct event_base* evbase) : _evb(evbase), _ctx(NULL) {
    _port   = 0;
    _partid = 0;
    _linkid = 0;
    memset(_addr, 0, sizeof(_addr));
    memset(_pass, 0, sizeof(_pass));
}

async_redis_client::~async_redis_client() {

}

void async_redis_client::set_addr(const char* addr) {
    if (!addr)
        return;

    int32_t size = sizeof(_addr);
    strncpy(_addr, addr, size - 1);
    _addr[size - 1] = '\0';
}

void async_redis_client::set_pass(const char* pass) {
    if (!pass)
        return;

    int32_t size = sizeof(_pass);
    strncpy(_pass, pass, size - 1);
    _pass[size - 1] = '\0';
}

void async_redis_client::set_port(const int32_t port) {
    _port = port;
}

void async_redis_client::set_partid(const int32_t partid) {
    _partid = partid;
}

int32_t async_redis_client::get_partid() {
    return _partid;
}

int32_t async_redis_client::connect() {
    if (!_evb) {
        ERROR("_evb is NULL");
        return -1;
    }

    if (_ctx) {
        redisAsyncFree(_ctx);
        _ctx = NULL;
    }

    _ctx = redisAsyncConnect(_addr, _port);
    if (!_ctx) {
        ERROR("redisAsyncConnect() failed");
        return -1;
    }

    if (_ctx->err) {
        ERROR("redisAsyncConnect() failed, errstr[%s]", _ctx->errstr);
        return -1;
    }

    _ctx->data = this;

    if (REDIS_ERR == redisLibeventAttach(_ctx, _evb)) {
        ERROR("redisLibeventAttach() failed");
        return -1;
    }

    if (REDIS_ERR == redisAsyncSetConnectCallback(_ctx, redis_connect_cb)) {
        ERROR("redisAsyncSetConnectCallback() failed");
        return -1;
    }

    if (REDIS_ERR == redisAsyncSetDisconnectCallback(_ctx, redis_disconnect_cb)) {
        ERROR("redisAsyncSetDisconnectCallback() failed");
        return -1;
    }

    return 0;
}

void async_redis_client::cb_connected() {
    INFO("Connected..., host[%d-%s:%d]", _partid, _addr, _port);
}

void async_redis_client::cb_disconnected() {
    INFO("Disconnected..., host[%d-%s:%d]", _partid, _addr, _port);
}

int32_t async_redis_client::cmd(redisCallbackFn *fn, void *privdata, const char *format, ...) {
    int32_t status = -1;
    if (!_ctx) {
        ERROR("invaild param: _ctx[%p]", _ctx);
        return status;
    }

    va_list ap;
    va_start(ap, format);
    status = redisvAsyncCommand(_ctx, fn, privdata, format, ap);
    va_end(ap);
    return status;
}

int32_t async_redis_client::is_connected() {
    if (!_ctx)
        return 0;

    return (_ctx->c.flags & REDIS_CONNECTED);
}

void async_redis_client::disconnect() {
    if (_ctx) {
        redisAsyncDisconnect(_ctx);
    }
}
