#ifndef __SINGLETION_TEMPLATE_H__
#define __SINGLETION_TEMPLATE_H__

#include <iostream>
#include <string>

template <typename T>
class singleton {
public:
    static T* instance() {
        if (!_instance) {
            _instance = new (T)();
            atexit(destory);
        }

        return _instance;
    }

private:
    static void destory() {
        delete _instance;
        _instance = NULL;
    }

    singleton() {

    }

    virtual ~singleton() {

    }

private:
    static T* volatile _instance;
};

template <typename T>
T* volatile singleton<T>::_instance = NULL;

#endif
