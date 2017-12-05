#ifndef __THREAD_BASE_H__
#define __THREAD_BASE_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef void* pthread_arg_t;

class abstract_thread {
public:
    abstract_thread();
    virtual ~abstract_thread();

public:
    virtual int32_t init();
    virtual int32_t final();
    virtual int32_t kill();
    virtual int32_t cancel();
    virtual int32_t run() = 0;

protected:
    void set_pthread_arg(pthread_arg_t arg);

public:
    pthread_t get_pthreadid();

private:
    pthread_t _thread_pid;
    pthread_attr_t _thread_attr;
    pthread_arg_t _trhead_arg;
};

#endif

