#include "log.h"
#include "time_helper.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int time_helper::get_current_time(timeval *t) {
    return gettimeofday(t, NULL);
}

unsigned time_helper::calc_time(time_t start_time,int hour,int min,int sec) {
    time_t t = start_time + hour*3600 + min*60 + sec;
    return (unsigned)t;
}

unsigned time_helper::get_day_hour(unsigned offset_hour, time_t now/* = 0*/) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_hour * 3600 - now_tm.tm_min * 60 - now_tm.tm_sec + offset_hour * 3600;
}

unsigned time_helper::get_day_timestamp(unsigned hour, unsigned minute, unsigned second, time_t now/*=0*/) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_hour * 3600 - now_tm.tm_min * 60 - now_tm.tm_sec + hour * 3600 + minute * 60 + second;
}

unsigned time_helper::get_cur_hour(time_t now /*= 0*/) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now_tm.tm_hour;
}

unsigned time_helper::thisDay(int offset, time_t now) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    now -= offset;
    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_hour * 3600 - now_tm.tm_min * 60 - now_tm.tm_sec + offset;
}

unsigned time_helper::nextDay(int offset, time_t now) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    now -= offset;

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_hour * 3600 - now_tm.tm_min * 60 - now_tm.tm_sec + 24 * 3600 + offset;
}

unsigned time_helper::get_cur_day_by_month(time_t now) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now_tm.tm_mday;
}

unsigned time_helper::get_cur_month_by_year(time_t now) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now_tm.tm_mon+1;
}

int time_helper::get_date_by_time(int32_t* year, int32_t* month, int32_t* day, time_t now/*=0*/) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);

    if (year) {
        *year = now_tm.tm_year + 1900;
    }

    if (month) {
        *month = now_tm.tm_mon + 1;
    }

    if (day) {
        *day = now_tm.tm_mday;
    }

    return 0;
}

int time_helper::get_datetime_by_time(int32_t* year, int32_t* month, int32_t* day, int32_t* hour,
                                      int32_t* min, int32_t* sec, time_t now/*=0*/) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);

    if (year) {
        *year = now_tm.tm_year + 1900;
    }

    if (month) {
        *month = now_tm.tm_mon + 1;
    }

    if (day) {
        *day = now_tm.tm_mday;
    }

    if (hour) {
        *hour = now_tm.tm_hour;
    }

    if (min) {
        *min = now_tm.tm_min;
    }

    if (sec) {
        *sec = now_tm.tm_sec;
    }

    return 0;
}

bool time_helper::is_same_hour(time_t t1, time_t t2) {
    if (t1 == 0 || t2 == 0) {
        return false;
    }

    tm now_tm1;
    tm now_tm2;
    localtime_r(&t1, &now_tm1);
    localtime_r(&t2, &now_tm2);
    uint32_t s1 = t1 - now_tm1.tm_min * 60 - now_tm1.tm_sec ; /// 当前小时的开始时间
    uint32_t s2 = t2 - now_tm2.tm_min * 60 - now_tm2.tm_sec ; /// 当前小时的开始时间
    return s1 == s2;
}

bool time_helper::is_same_month_by_year(time_t t1, time_t t2) {
    tm now_tm1;
    tm now_tm2;
    localtime_r(&t1, &now_tm1);
    localtime_r(&t2, &now_tm2);
    return now_tm1.tm_year == now_tm2.tm_year && now_tm1.tm_mon == now_tm2.tm_mon;
}

unsigned time_helper::nextHalfHour(time_t now) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_min * 60 + (now_tm.tm_min / 30 * 30 + 30) * 60;
}

unsigned time_helper::nextHour(time_t now) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_min * 60 + 3600;
}

unsigned time_helper::thisWeek(int offset /* = 0 */, time_t now /* = 0 */) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    now -= offset;
    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_wday * 24 * 3600 - now_tm.tm_hour * 3600 - now_tm.tm_min * 60 - now_tm.tm_sec + offset;
}

unsigned time_helper::nextWeek(int offset /* = 0 */, time_t now /* = 0 */)
{
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    now -= offset;
    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_wday * 24 * 3600 - now_tm.tm_hour * 3600 - now_tm.tm_min * 60 - now_tm.tm_sec + 7 * 24 * 3600 + offset;
}

unsigned time_helper::next2Minute(time_t now /* = 0 */) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_min * 60 - now_tm.tm_sec + (now_tm.tm_min / 2 + 1) * 2 * 60;
}

unsigned time_helper::getWeek(time_t now/*=0*/) {
    if (now == 0) {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now_tm.tm_wday;
}

unsigned time_helper::get_timestamp_by_day(int hour, int minute, time_t now) {
    if (now == 0)  {
        struct timeval tv;
        get_current_time(&tv);
        now = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&now, &now_tm);
    return now - now_tm.tm_hour * 3600 - now_tm.tm_min * 60 - now_tm.tm_sec + hour*3600+minute*60;
}

void time_helper::showTime(time_t t) {
    if (t == 0) {
        struct timeval tv;
        get_current_time(&tv);
        t = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&t, &now_tm);
    showTime(now_tm);
}

void time_helper::showTime(tm t) {
    printf("%04d-%02d-%02d-%02d-%02d-%02d\n", t.tm_year + 1900,
           t.tm_mon + 1,
           t.tm_mday,
           t.tm_hour,
           t.tm_min,
           t.tm_sec);
}

int time_helper::timezone_offset(void) {
    return (timezone);
}

bool time_helper::is_same_day(unsigned int t1, unsigned int t2/*=0*/) {
    if (t2 == 0) {
        struct timeval tv;
        get_current_time(&tv);
        t2 = tv.tv_sec;
    }

    unsigned int t1_s = time_helper::get_day_hour(0, t1);
    unsigned int t2_s = time_helper::get_day_hour(0, t2);
    return t1_s == t2_s;
}

static time_t cached_time;
void time_helper::set_cached_time(time_t t) {
    cached_time = t;
}

time_t time_helper::get_cached_time() {
    return cached_time;
}

uint32_t time_helper::get_time(int year, int month, int day) {
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    return mktime(&tm);
}

uint32_t time_helper::get_time(int year, int month, int day, int hour, int minute, int second) {
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    return mktime(&tm);
}

std::string time_helper::get_date(time_t t/*=0*/, const char* separator/*="-"*/) {
    if (t == 0) {
        struct timeval tv;
        get_current_time(&tv);
        t = tv.tv_sec;
    }

    tm now_tm;
    localtime_r(&t, &now_tm);

    char buffer[64];
    if (separator)
        snprintf(buffer, sizeof(buffer), "%04d%s%02d%s%02d", now_tm.tm_year + 1900, separator, now_tm.tm_mon + 1, separator, now_tm.tm_mday);
    else
        snprintf(buffer, sizeof(buffer), "%04d%02d%02d", now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday);

    return std::string(buffer);
}

bool time_helper::valid_datetime(int year, int month, int day, int hour, int minute, int second) {
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
        return false;

    if (year < 1900)
        return false;

    if (month < 1 || month > 12)
        return false;

    if (day < 1 || day > 31)
        return false;

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;

    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29)
                return false;
        }
        else {
            if (day > 28)
                return false;
        }
    }

    return true;
}

//毫秒级别时间
int64_t time_helper::system_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_usec;
}

//秒数级别时间
int32_t time_helper::system_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

int32_t time_helper::get_differ_day(time_t first, time_t second) {
    unsigned int t1_s = time_helper::get_day_hour(0, first);
    unsigned int t2_s = time_helper::get_day_hour(0, second);
	return t2_s > t1_s ? (t2_s - t1_s)/86400: 0;
}
