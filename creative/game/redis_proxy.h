#ifndef __REDIS_PROXY_H__
#define __REDIS_PROXY_H__

#include "noncopyable.h"
#include "async_redis_client.h"

class redis_proxy : public async_redis_client, private noncopyable {
public:
    redis_proxy(struct event_base* evbase);
    virtual ~redis_proxy();

    void test();
};

#endif

