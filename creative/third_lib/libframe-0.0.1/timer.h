#ifndef __TIMER_TEMPLATE_H__
#define __TIMER_TEMPLATE_H__

#include <sys/time.h>
#include <stdint.h>

//INTERVAL为间隔，毫秒表示
template<uint32_t INTERVAL>
class CTimer {
public:
    CTimer() {
        Reset();
    }

    inline uint64_t GetCurrTime() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }

    inline bool CheckAndSet(uint64_t currTime = 0) {
        if (currTime == 0)
            currTime = GetCurrTime();

        if (currTime >= m_nextTime) {
            m_nextTime = currTime + INTERVAL;
            return true;
        }

        return false;
    }

    inline bool Check(uint64_t currTime = 0) {
        if (currTime == 0) {
            currTime = GetCurrTime();
        }

        return currTime >= m_nextTime;
    }

    inline void SetNextHitTime(uint64_t nextTime) {
        m_nextTime = nextTime;
    }

    inline uint64_t GetNextTime() {
        return m_nextTime;
    }

    inline void Reset(uint64_t currTime = 0) {
        if (currTime == 0)
            currTime = GetCurrTime();

        m_nextTime = currTime + INTERVAL;
    }

    inline void Delay(uint32_t nTick) {
        m_nextTime += nTick;
    }

private:
    uint64_t m_nextTime;    //ms
};

#endif
