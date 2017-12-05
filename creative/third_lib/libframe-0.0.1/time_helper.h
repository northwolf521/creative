#ifndef __TIME_HELPER_H__
#define __TIME_HELPER_H__

#include <time.h>
#include <string>
#include <stdint.h>

class time_helper
{
public:
    static void set_cached_time(time_t t);
    static time_t get_cached_time();
    static std::string print_time_string(time_t t,bool is_print = false);
    //t=0: 表示当前时间
    static std::string get_date(time_t t = 0, const char* separator = "-");
    static int      get_current_time(timeval *t);
    static unsigned calc_time(time_t start_time,int hour,int min,int sec);
    static bool     get_duration_time(time_t time_start, time_t time_end, int& hour, int& min, int& sec);
    //获取now所对应的整数天+hour对应的时间戳
    static unsigned get_day_hour(unsigned hour, time_t now = 0);
    static unsigned get_day_timestamp(unsigned hour, unsigned minute, unsigned second, time_t now=0);
    //result: 0~23
    static unsigned get_cur_hour(time_t now = 0);
    //next offset time!
    static unsigned thisDay(int offset = 0, time_t now = 0);
    //next offset time!
    static unsigned nextDay(int offset = 0, time_t now = 0);
    //获取当前时间是本月的第几天
    static unsigned get_cur_day_by_month(time_t now=0);
    //判断两时间戳是否为同一年中的同一月
    static bool     is_same_month_by_year(time_t t1, time_t t2);
    static unsigned nextHalfHour(time_t now = 0);
    static unsigned nextHour(time_t now = 0);
    //offset为0则返回本周周日0点时间戳，一周是周日开始周六结束
    static unsigned thisWeek(int offset = 0, time_t now = 0);
    static unsigned nextWeek(int offset = 0, time_t now = 0);
    static unsigned next2Minute(time_t now = 0);
    static unsigned getWeek(time_t now=0);
    static unsigned get_timestamp_by_day(int hour, int minute, time_t now = 0);
    static void     showTime(time_t t = 0);
    static void     showTime(tm t);
    static int      timezone_offset(void);
    static bool     is_same_day(unsigned int t1, unsigned int t2=0);
    static void     sleep(int micro_secs);
    static uint32_t get_time(int year, int month, int day);
    static uint32_t get_time(int year, int month, int day, int hour, int minute, int second);
    static unsigned get_cur_month_by_year(time_t now);
    static int      get_date_by_time(int32_t* year, int32_t* month, int32_t* day, time_t now=0);  /// 根据时间戳返回年/月/日
    static int      get_datetime_by_time(int32_t* year, int32_t* month, int32_t* day, int32_t* hour, int32_t* min, int32_t* sec, time_t now=0);
    static bool     is_same_hour(time_t t1, time_t t2);
    static bool     valid_datetime(int year, int month, int day, int hour, int minute, int second);//毫秒级别时间
    static int64_t  system_timestamp();
    static int32_t  system_seconds();
	//相差多少天(second>=first)
	static int32_t get_differ_day(time_t first, time_t second);
};

#endif
