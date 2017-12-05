#ifndef __ABSTRACT_LOCK_H__
#define __ABSTRACT_LOCK_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "noncopyable.h"

class abstract_lock {
public:
    virtual void lock()   = 0;
    virtual void unlock() = 0;
    virtual void rdlock() = 0;
    virtual void wrlock() = 0;
};

class pthread_mutex : public abstract_lock, private noncopyable {
public:
    pthread_mutex();
    ~pthread_mutex();

    void lock();
    void unlock();

private:
    void rdlock();
    void wrlock();

private:
    pthread_mutex_t mtx;
    pthread_mutexattr_t attr;
};

class pthread_rwlock : public abstract_lock, private noncopyable {
public:
    pthread_rwlock();
    ~pthread_rwlock();

    void unlock();
    void rdlock();
    void wrlock();

private:
    void lock();

private:
    pthread_rwlock_t rwlock;
    pthread_rwlockattr_t attr;
};

#endif