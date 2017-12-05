#ifndef __THREAD_MANAGER_H__
#define __THREAD_MANAGER_H__

#include <iostream>
#include <string>
#include <map>
#include "noncopyable.h"
#include "abstract_thread.h"

typedef std::map<int32_t, abstract_thread*> thread_map_t;
class thread_manager : private noncopyable {
public:
    thread_manager();
    virtual ~thread_manager();

    int32_t start();
    int32_t stop();
    int32_t add_thread(abstract_thread* tb);
    int32_t del_thread(abstract_thread* tb);

private:
    thread_map_t _thread_map;
};

#endif