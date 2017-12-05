#ifndef __ABSTRACT_DEQUE_H__
#define __ABSTRACT_DEQUE_H__

#include <iostream>
#include <string>
#include <deque>
#include <functional>
#include "lock.h"

template<class T>
class abstract_deque {
public:
    abstract_deque() : m_lock(nullptr) {
        m_lock = new pthread_mutex();
    }
    virtual ~abstract_deque() {
        if (m_lock) {
            delete m_lock;
            m_lock = nullptr;
        }
    }
    T* pop() {
        T* ptr = nullptr;
        m_lock->lock();

        if (m_task_queue.size() > 0) {
            ptr = m_task_queue.front();
            m_task_queue.pop_front();
        }

        m_lock->unlock();
        return ptr;
    }
    void push(T* ptr) {
        if (ptr) {
            m_lock->lock();
            m_task_queue.push_back(ptr);
            m_lock->unlock();
        }
    }
    int32_t size() {
        size_t len = 0;
        m_lock->lock();
        len = m_task_queue.size();
        m_lock->unlock();
        return len;
    }
    void clear() {
        m_lock->lock();
        m_task_queue.clear();
        m_lock->unlock();
    }
    void remove(T* ptr) {
        if (ptr) {
            m_lock->lock();
            auto iter = m_task_queue.find(ptr);
            if (iter != m_task_queue.end()) {
                m_task_queue.erase(iter);
            }
            m_lock->unlock();
        }
    }

private:
    std::deque<T*> m_task_queue;
    pthread_mutex* m_lock;
};

#endif
