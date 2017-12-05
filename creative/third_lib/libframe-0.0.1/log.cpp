#include "log.h"
#include "log.h"

vgame::VGameLog g_vgame_log;

namespace vgame
{
int VGameLog::Init(const char *conf_path, const char *app_name) {
    if (log_ctx_) {
        snprintf(err_string, LOG_ERROR_STRING_LEN, "Repeat init tlog!");
        return -1;
    }

    //根据配置文件初始化日志句柄
    log_ctx_ = tlog_init_from_file(conf_path);
    if (log_ctx_ == NULL) {
        snprintf(err_string, LOG_ERROR_STRING_LEN, "Init tlog from file:%s failed!", conf_path);
        return -1;
    }

    int ret = InitCategory();
    if (ret != 0) {
        //这里不要填err_string了，上面的Init里会填具体的err_string
        //snprintf(err_string, LOG_ERROR_STRING_LEN, "Init category failed");
        return ret;
    }

    ret = tlog_set_module_name(log_ctx_, app_name);
    if (ret != 0) {
        snprintf(err_string, LOG_ERROR_STRING_LEN, "Set module name:%s failed", app_name);
        return ret;
    }

    return ret;
}

int VGameLog::Reload(const char *conf_path, const char *app_name) {
    if (log_ctx_) {
        //这个函数的文档是错的，TSF4G的人说，其实这个返回没意义，不需要判断，额，那你要返回一个值干嘛
        int ret = tlog_fini_ctx(&log_ctx_);
        if (ret != 0) {
            snprintf(err_string, LOG_ERROR_STRING_LEN, "free log ctx failed:%d", ret);
        }
    }

    log_ctx_ = NULL;
    return Init(conf_path, app_name);
}

#define INIT_CATEGORY(log_name, category) { \
    category = tlog_get_category(log_ctx_, log_name.c_str()); \
    if (category == NULL) { \
        snprintf(err_string, LOG_ERROR_STRING_LEN, "Init category failed!, log_name=%s", log_name.c_str()); \
        return -1; \
    } \
}

int VGameLog::InitCategory() {
    INIT_CATEGORY(fatal_log_name_, fatal_category_);
    INIT_CATEGORY(err_log_name_, err_category_);
    INIT_CATEGORY(warn_log_name_, warn_category_);
    INIT_CATEGORY(info_log_name_, info_category_);
    INIT_CATEGORY(debug_log_name_, debug_category_);
    INIT_CATEGORY(trace_log_name_, trace_category_);
    INIT_CATEGORY(oss_log_name_, oss_category_);
    INIT_CATEGORY(stat_log_name_, stat_category_);
    return 0;
}

}/* end of namespace vgame */
