#ifndef __NON_COPYABLE_H__
#define __NON_COPYABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class noncopyable {
protected:
    noncopyable() {}
    virtual ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    noncopyable& operator = (const noncopyable&);
};

#endif

