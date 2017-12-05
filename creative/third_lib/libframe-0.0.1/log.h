#ifndef __VLOG_H__
#define __VLOG_H__

#include "tlog/tlog.h"
#include "tloghelp/tlogload.h"
#include "tlog/tlog_category.h"
#include <string>

namespace vgame {

class VGameLog {
public:
    VGameLog():
        log_ctx_(NULL),
        fatal_category_(NULL),
        err_category_(NULL),
        warn_category_(NULL),
        info_category_(NULL),
        debug_category_(NULL),
        trace_category_(NULL),
        oss_category_(NULL),
        stat_category_(NULL),
        fatal_log_name_("fatal"),
        err_log_name_("error"),
        warn_log_name_("warn"),
        info_log_name_("info"),
        debug_log_name_("debug"),
        trace_log_name_("trace"),
        oss_log_name_("oss"),
        stat_log_name_("statistics") {
        snprintf(strErrDes, LOG_ERROR_STRING_LEN, "not Init");
    }

    ~VGameLog() {
        tlog_fini_ctx(&log_ctx_);
    }

    //初始化
    int Init(const char *conf_path, const char *app_name);
    //重新加载
    int Reload(const char *conf_path, const char *app_name);
    //初始日志分类
    int InitCategory();
    //错误描述
    const char* GetErrMsg() {
        return strErrDes;
    }

    static std::string FormatTimeStr() {
        time_t time_now = time(NULL);
        struct tm tm_now;
        localtime_r(&time_now, &tm_now);
        char temp_str[32] = {0};
        sprintf(temp_str, "%04d-%02d-%02d %02d:%02d:%02d",
                tm_now.tm_year + 1900,
                tm_now.tm_mon + 1,
                tm_now.tm_mday,
                tm_now.tm_hour,
                tm_now.tm_min,
                tm_now.tm_sec);
        return temp_str;
    }

public:
    //tlog日志的句柄
    LPTLOGCTX log_ctx_;
    //各个category的指针
    LPTLOGCATEGORYINST fatal_category_;
    LPTLOGCATEGORYINST err_category_;
    LPTLOGCATEGORYINST warn_category_;
    LPTLOGCATEGORYINST info_category_;
    LPTLOGCATEGORYINST debug_category_;
    LPTLOGCATEGORYINST trace_category_;
    LPTLOGCATEGORYINST oss_category_;
    LPTLOGCATEGORYINST stat_category_;
    //各个category在tlog日志里面配置的名字
    std::string fatal_log_name_;
    std::string err_log_name_;
    std::string warn_log_name_;
    std::string info_log_name_;
    std::string debug_log_name_;
    std::string trace_log_name_;
    std::string oss_log_name_;
    std::string stat_log_name_;
    //记录错误日志
    static const int LOG_ERROR_STRING_LEN = 256;
    char strErrDes[LOG_ERROR_STRING_LEN];
};

}/*end of namespace vgame*/

extern vgame::VGameLog g_vgame_log;

#define TRACE(format, args...) {\
            tlog_trace(g_vgame_log.trace_category_, 0, 0, format, ##args);\
        }

#define DBG(format, args...) {\
            tlog_debug(g_vgame_log.debug_category_, 0, 0, format, ##args);\
        }

#define INFO(format, args...) {\
            tlog_info(g_vgame_log.info_category_, 0, 0, format, ##args);\
        }

#define WARN(format, args...) {\
            tlog_warn(g_vgame_log.warn_category_, 0, 0, format, ##args);\
        }

#define ERROR(format, args...) {\
            tlog_error(g_vgame_log.err_category_, 0, 0, format, ##args);\
        }

#define FATAL(format, args...) {\
            tlog_error(g_vgame_log.fatal_category_, 0, 0, format, ##args);\
        }

#define OSS(format, args...) {\
            tlog_info(g_vgame_log.oss_category_, 0, 0, format, ##args);\
        }

#define STAT(log_id, svr_id, format, args...) {\
            tlog_info(g_vgame_log.stat_category_, 0, 0, format, ##args);\
        }

#endif /* __VLOG_H__ */
