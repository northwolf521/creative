#include "server_node_base.h"

static void eventlog_cb(int loglv, const char *msg) {
    if (!msg) {
        return;
    }

    if (loglv == EVENT_LOG_ERR) {
        ERROR("%s", msg);
        return;
    }

    if (loglv == EVENT_LOG_WARN) {
        WARN("%s", msg);
        return;
    }

    if (loglv == EVENT_LOG_MSG) {
        INFO("%s", msg);
        return;
    }

    DBG("%s", msg);
    return;
}

server_node_base::server_node_base() {
    _evbase   = NULL;
    _evconfig = NULL;
}

server_node_base::~server_node_base() {
    if (_evconfig) {
        event_config_free(_evconfig);
        _evconfig = NULL;
    }

    if (_evbase) {
        event_base_free(_evbase);
        _evbase = NULL;
    }
}

struct event_base* server_node_base::event_base_ptr() {
    return _evbase;
}

int32_t server_node_base::init(int32_t argc, char** argv) {
    event_set_log_callback(eventlog_cb);

    _evconfig = event_config_new();
    if (!_evconfig) {
        ERROR("event_evconfig_new() failed, error[(%d)-%s]", errno, strerror(errno));
        return -1;
    }

    if (-1 == event_config_require_features(_evconfig, EV_FEATURE_ET | EV_FEATURE_O1)) {
        ERROR("event_config_require_features() failed, error[(%d)-%s]", errno, strerror(errno));
        return -1;
    }

    if (-1 == event_config_set_flag(_evconfig, EVENT_BASE_FLAG_NOLOCK | EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST)) {
        ERROR("event_config_set_flag() failed, error[(%d)-%s]", errno, strerror(errno));
        return -1;
    }

    _evbase = event_base_new_with_config(_evconfig);
    if (!_evbase) {
        ERROR("event_base_new_with_evconfig failed, error[(%d)-%s]", errno, strerror(errno));
        return -1;
    }

    return 0;
}

int32_t server_node_base::loop(int32_t flags) {
    return event_base_loop(_evbase, flags);
}

int32_t server_node_base::final() {
    for (auto it = _signal_map.begin(); it != _signal_map.end(); it++) {
        struct event* ev = (*it).second;
        if (ev) {
            event_free(ev);
        }
    }

    _signal_map.clear();
    return 0;
}

int32_t server_node_base::stop() {
    return event_base_loopbreak(_evbase);
}

int32_t server_node_base::add_period_cb(struct event* ev, int32_t ms, event_callback_fn fn, void* arg) {
    if (!ev || !_evbase || !fn)
        return -1;

    struct timeval tv;
    tv.tv_sec  = ms / 1000;
    tv.tv_usec = ms % 1000 * 1000;
    if (-1 == event_assign(ev, _evbase, -1,  EV_PERSIST, fn, arg)) {
        ERROR("event_assign() failed");
        return -1;
    }

    return event_add(ev, &tv);
}

int32_t server_node_base::add_timeout_cb(struct event* ev, int32_t ms, event_callback_fn fn, void* arg) {
    if (!ev || !_evbase || !fn)
        return -1;

    struct timeval tv;
    tv.tv_sec  = ms / 1000;
    tv.tv_usec = ms % 1000 * 1000;
    if (-1 == event_assign(ev, _evbase, -1, 0, fn, arg)) {
        ERROR("event_assign() failed");
        return -1;
    }

    return event_add(ev, &tv);
}

int32_t server_node_base::gettimeofday_cached(struct timeval* tv) {
    return event_base_gettimeofday_cached(_evbase, tv);
}

int32_t server_node_base::add_signal_handler(signal_fn fn, int32_t signo, void* pridata) {
    auto it = _signal_map.find(signo);
    if (it != _signal_map.end()) {
        struct event* ev = (*it).second;
        if (ev)
            event_free(ev);
    }

    struct event* _sigev = evsignal_new(_evbase, signo, fn, pridata);
    if (!_sigev) {
        ERROR("Register %d handler failed", signo);
        return -1;
    }

    if (-1 == event_add(_sigev, NULL)) {
        event_free(_sigev);
        ERROR("event_add() %d handler failed", signo);
        return -1;
    }

    _signal_map.insert(std::pair<int32_t, struct event*>(signo, _sigev));
    return 0;
}

int32_t server_node_base::del_signal_handler(int32_t signo) {
    auto it = _signal_map.find(signo);
    if (it == _signal_map.end())
        return -1;

    struct event* ev = (*it).second;
    if (ev)
        event_free(ev);

    _signal_map.erase(it);
    return 0;
}