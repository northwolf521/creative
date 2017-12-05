#ifndef __TASK_DEQUE_H__
#define __TASK_DEQUE_H__

#include "noncopyable.h"
#include "abstract_deque.h"
#include "task_info.h"

class task_deque : public abstract_deque<task_info>, private noncopyable {
public:
    task_deque();
    virtual ~task_deque();

};
#endif

