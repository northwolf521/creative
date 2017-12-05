#ifndef __TASK_INFO_H__
#define __TASK_INFO_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

class task_info {
public:
    task_info();
    virtual ~task_info();
    virtual void proc();

    void set_task_type(int32_t type) {
        this->_task_type = type;
    }

    int32_t get_task_type() {
        return this->_task_type;
    }

private:
    int32_t _task_type;
};

#endif

