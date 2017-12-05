#include "abstract_thread.h"
#include "log.h"

static pthread_arg_t thread_callback_func(pthread_arg_t arg) {
    auto target = (abstract_thread*)arg;
    if (!target) {
        ERROR("pthread exit");
        pthread_exit(0);
    }

    for (;;) {
        if (target) {
            target->run();
            pthread_testcancel();
        }

        usleep(50000);
    }

    pthread_exit(0);
}

abstract_thread::abstract_thread() {
    _trhead_arg = NULL;
}

abstract_thread::~abstract_thread() {

}

int32_t abstract_thread::init() {
    int32_t ret = pthread_attr_init(&_thread_attr);
    if (ret != 0) {
        ERROR("pthread_attr_init() failed. ret[%d]", ret);
        return -1;
    }

    ret = pthread_attr_setdetachstate(&_thread_attr, PTHREAD_CREATE_DETACHED);
    if (ret != 0) {
        ERROR("pthread_attr_setdetachstate() failed, ret[%d]", ret);
        return -1;
    }

    ret = pthread_create(&_thread_pid, &_thread_attr, &thread_callback_func, _trhead_arg);
    if (ret != 0) {
        ERROR("pthread_create() failed. ret[%d]", ret);
        return -1;
    }

    return 0;
}

int32_t abstract_thread::final() {
    pthread_attr_destroy(&_thread_attr);
    return 0;
}

int32_t abstract_thread::kill() {
    pthread_kill(_thread_pid, SIGKILL);
    return 0;
}

int32_t abstract_thread::cancel() {
    pthread_cancel(_thread_pid);
    return 0;
}

void abstract_thread::set_pthread_arg(pthread_arg_t arg) {
    _trhead_arg = arg;
}

pthread_t abstract_thread::get_pthreadid() {
    return _thread_pid;
}