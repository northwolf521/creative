#include "thread_manager.h"
#include "log.h"

thread_manager::thread_manager() {

}

thread_manager::~thread_manager() {

}

int32_t thread_manager::start() {
    for (auto iter = _thread_map.begin(); iter != _thread_map.end(); iter++) {
        auto target = (*iter).second;
        if (!target)
            continue;

        if (0 != target->init()) {
            ERROR("init() failed, target[%p]", target);
            return -1;
        }
    }

    return 0;
}

int32_t thread_manager::stop() {
    for (auto iter = _thread_map.begin(); iter != _thread_map.end(); iter++) {
        auto target = (*iter).second;
        if (!target)
            continue;

        if (0 != target->cancel()) {
            ERROR("cancel() failed, target[%p]", target);
        }

        if (0 != target->final()) {
            ERROR("final() failed, target[%p]", target);
        }

        delete target;
    }

    _thread_map.clear();
    DBG("thread_manager::stop() succ");
    return 0;
}

int32_t thread_manager::add_thread(abstract_thread* tb) {
    if (!tb) {
        ERROR("invaild tb");
        return -1;
    }

    _thread_map[tb->get_pthreadid()] = tb;
    return 0;
}

int32_t thread_manager::del_thread(abstract_thread* tb) {
    if (!tb) {
        ERROR("invaild tb");
        return -1;
    }

    _thread_map.erase(tb->get_pthreadid());
    return 0;
}
