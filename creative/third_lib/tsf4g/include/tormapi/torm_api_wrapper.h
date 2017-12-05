

/**
*
* @file     torm_api_wrapper.h
* @brief    在torm 底层接口和bus的基础上进行封装,提供一套更上层的 
* 应用程序接口,避免应用程序了解torm内部协议细节和bus接口细节,
* 提高系统的可扩展性.
* 
* @author vinsonzuo
* @version 1.0
* @date 2009-10-16 
*
*
* Copyright (c)  2009, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/



#ifndef TORM_API_WRAPPER_H
#define TORM_API_WRAPPER_H

#include "tormapi/torm_protocol.h"
#include "tormapi/torm_protocol_api.h"
//#include "tormapi/torm_log.h"

#define TORM_MIN_CALLBACK_ID 1	/* 注册torm 回调函数id 最小值*/
#define TORM_MAX_CALLBACK_ID 1023  /* 注册torm 回调函数id 最大值*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
* torm 相关参数名称
*/
enum TORM_OPTION_NAME
{
    TORM_OPT_NO, /* 无效参数 */
    TORM_OPT_MAX_NET_BUFF_LEN, /* 消息最大长度，默认65536*/
    TORM_OPT_RUN_MODE, /* 运行模式，目前只支持异步，默认异步*/
    TORM_OPT_CONN_MODE, /* 连接模式，目前支持连接proxy和直连tormsvr*/
};
    
/****************************************/
/**
* torm 操作类型
*/
enum TORM_OPERATE_TYPE
{
    TORM_OPERATE_TYPE_NO,
    TORM_OPERATE_TYPE_INSERT, /* 对应数据库insert操作 */
    TORM_OPERATE_TYPE_UPDATE, /* 对应数据库update操作 */
    TORM_OPERATE_TYPE_SELECT, /* 对应数据库select操作 */
    TORM_OPERATE_TYPE_DELETE, /* 对应数据库delete操作 */
    TORM_OPERATE_TYPE_SELECT_COUNT, /* 独立出来的获得记录行数的操作 */
    TORM_OPERATE_TYPE_SVRINFO, /* 获得tormsvr服务进程状态信息的操作 */
    TORM_OPERATE_TYPE_SELECT_TABLE, /* 指定表名称查询*/
};

struct st_torm;
typedef struct st_torm        TORM; 

struct st_torm_result;
typedef struct st_torm_result        TORM_RESULT; 

struct st_torm_operate_insert;
typedef struct st_torm_operate_insert   TORM_OPERATE_INSERT;

struct st_torm_operate_update;
typedef struct st_torm_operate_update   TORM_OPERATE_UPDATE;

struct st_torm_operate_delete;
typedef struct st_torm_operate_delete   TORM_OPERATE_DELETE;

struct st_torm_operate_select;
typedef struct st_torm_operate_select   TORM_OPERATE_SELECT;

struct st_torm_operate_select_table;
typedef struct st_torm_operate_select_table   TORM_OPERATE_SELECT_TABLE;

struct st_torm_operate_select_count;
typedef struct st_torm_operate_select_count   TORM_OPERATE_SELECT_COUNT;

struct st_torm_operate_svrinfo;
typedef struct st_torm_operate_svrinfo   TORM_OPERATE_SVRINFO;

union st_torm_operate;
typedef union st_torm_operate   TORM_OPERATE;


/**
* torm 参数结构
*/
typedef struct st_torm_options 
{
    /**
    * 运行模式，
    * 1: 同步模式，目前不支持
    * 0: 异步模式，在该模式下，在发送请求消息后立刻返回，
    *   在torm_do_recv时候检查返回情况
    */
    unsigned int run_mode;
    
    
    /** 连接模式，区分连接proxy和直连tormsvr
    * 1: 连接proxy中转，会自动在torm协议包前增加proxy包头
    * 0: 直接连接tormsvr
    */
    unsigned int conn_mode;
    
    /**
    * 最大的消息包长度
    * > 0：最大包长
    * 其他: 使用默认值TORM_MAX_PKG_LEN
    */           
    unsigned int max_net_buff_len;
    
}TORM_OPTION ;

/**
* torm 操作元数据定义
*/
typedef struct st_torm_meta
{
    char name[TORM_MAX_META_NAME];   /* 元数据名称 */
    unsigned int verion;    /* 元数据版本*/
    unsigned int entry_num;  /* 操作项数目*/                 
    char entrys[TORM_MAX_ENTRYLIST][TORM_MAX_META_NAME]; /* 操作项*/
}TORM_META; 


/**
* torm 操作返回基本结果
*/
struct st_torm_result
{
   int operate_type;   /* 操作类型，bind to TORM_OPERATE_TYPE */
   int result;    /* 操作结果 */
};

/**
* torm 插入操作
*/
struct st_torm_operate_insert
{
    TORM_META  meta;   /*操作针对的元数据*/
    int callback_id;  /*操作返回处理函数*/
    unsigned int res_flag;
};

/**
* torm 更新操作
*/
struct st_torm_operate_update
{
    TORM_META  meta;   /* 操作针对的元数据 */ 
    char  where[TORM_MAX_WHERE_DEF] ;   /* where条件，如果没有，必须为空字符串*/
    int do_insert;    /* 是否在更新记录不存在的情况下插入新的记录*/
    int callback_id;  /*操作返回处理函数*/
    unsigned int res_flag;
};

/**
* torm 删除操作
*/
struct st_torm_operate_delete
{
    TORM_META  meta;   /* 操作针对的元数据 */ 
    char where[TORM_MAX_WHERE_DEF];  /* 如果没有自定义条件，必须设为空字符串 */
    int callback_id;  /*操作返回处理函数*/
    unsigned int res_flag;
};

/**
* torm 读取操作
*/
struct st_torm_operate_select
{
    TORM_META  meta;   /* 操作针对的元数据 */ 
    char where[TORM_MAX_WHERE_DEF];   /* 如果没有自定义条件，必须设为空字符串 */
    int offset;       /* 大于等于零有效 */
    int limit;        /* 大于零有效*/
    int callback_id;  /*操作返回处理函数*/
    unsigned int res_flag;
};

/**
* torm 从指定的数据表读取操作
*/
struct st_torm_operate_select_table
{
    TORM_META  meta;   /* 操作针对的元数据 */ 
	char table_name[TORM_MAX_TABLE_NAME];  /*数据表名称*/
	int insert_cache;  /*是否将查询结果加载到cache*/
    char where[TORM_MAX_WHERE_DEF];   /* 如果没有自定义条件，必须设为空字符串 */
    int offset;       /* 大于等于零有效 */
    int limit;        /* 大于零有效*/
    int callback_id;  /*操作返回处理函数*/
    unsigned int res_flag;
};


/**
* torm 查找记录行总数操作
*/
struct st_torm_operate_select_count
{
    TORM_META  meta;   /* 操作针对的元数据 */ 
    char where[TORM_MAX_WHERE_DEF];   /* 如果没有自定义条件，必须设为空字符串 */
    int callback_id;  /*操作返回处理函数*/
};

/**
* torm 查询服务进程状态的操作
*/
struct st_torm_operate_svrinfo
{
    char name[TORM_MAX_META_NAME];   /* 元数据名称 */
    int callback_id;  /*操作返回处理函数*/
};

/**
* torm 操作
*/
union st_torm_operate
{
    TORM_OPERATE_INSERT  opt_insert;
    TORM_OPERATE_UPDATE  opt_update;
    TORM_OPERATE_DELETE  opt_delete;
    TORM_OPERATE_SELECT  opt_select;
    TORM_OPERATE_SELECT_COUNT  opt_select_count;
    TORM_OPERATE_SVRINFO  opt_svrinfo;
	TORM_OPERATE_SELECT_TABLE opt_select_table;
};


/**
* 返回处理函数
*@param[in]    torm torm句柄
*@param[in]    async_data 异步数据，把执行操作时传入的数据返回
*@param[in]    async_len 异步数据长度，最长1024
*@param[out]    result 操作基本结果
*@return, 0: 处理成功
        其他: 处理失败
*/
typedef int (*RES_FUNC) (TORM* torm, char *async_data, int async_len, TORM_RESULT* result);

/******************************************/
/**
* torm操作详细结果，内部数据结构
*/
typedef struct st_torm_result_in
{
    TORMPKG res_pkg; /* 操作返回包 */

    TORM_OPERATE operate; /* 从返回结果中重构操作信息*/
} TORM_RESULT_IN ;

/******************************************/
/**
* torm回调函数列表，内部数据结构
*/
typedef struct st_torm_callback_mgr
{
    RES_FUNC func_list[TORM_MAX_CALLBACK_ID+1]; /* 数组下标为对应的回调函数id*/
    
} TORM_CALLBACK_MGR ;


/*********************************************/
/**
* torm 总数据结构，内部数据结构
*/
struct st_torm
{
    TORMPROTOCOLHANDLE proto_handle;   /* LPTORMPROHANDLER */
    int bus_handle;     /* bus通信句柄*/

    TORMPKG req_pkg;   /* 请求协议包*/

    char* net_buff;        /* 网络协议空间地址*/
    int net_buff_len;    /* 网络协议空间长度*/

    TORM_OPTION options;  /* torm 参数配置 */
    
    TORM_RESULT_IN  result;   /* 当前操作返回结果 */
    
    TORM_CALLBACK_MGR callback_tab;  /* 回调函数管理 */

    int callback_id;  /* 默认操作返回处理函数 */ 

    int proc_id;   /* 保留进程的bus id*/
	int svr_id;    /*保留server的进程id*/
};
/**********************************************/

/**@defgroup  TORM  TORM_协议和bus封装API
* @{
*@note Torm 协议和bus封装API的思想：
*  - 对应用屏蔽torm协议内容，便于torm协议的扩展和版本升级，应用不应该直接对协议进行操作
*  - 对应用屏蔽bus通信部分，应用只需要初始化时将自身进程号和tormsvr进程号建立连接即可
*  - 将应用需求定义为对某个应用数据进行某个操作
*  - 应用的主要工作是定义操作，即定义要对数据要进行操作的类型，字段，限制条件以及操作结果返回后的处理函数ID等
*  - 应用定义的操作中用到的结果返回处理函数需要在torm中进行注册
*  - 应用通过接口获取返回结果
*  - 接口使用流程请参考相关文档和实例程序
*  - 应用定义操作的时候可以直接对操作结构体字段进行付值，为了方便版本升级，应该先使用torm_operate_init来初始化操作
*/

/**
* 设置参数值,支持的参数类型请参考TORM_OPTION_NAME
*@param[in]    torm torm句柄
*@param[in]    opt_name 参数名
*@param[in]    opt_val 参数数据地址
*@param[in]      opt_len 参数数据长度
*@return, 0 成功，其他，错误
*/
int  torm_options(TORM *torm, int opt_name, void* opt_val, int opt_len);

/**
* 设置操作员元数据的名称
*@param[in]    meta meta句柄
*@param[in]    name 元数据名称
*@return, 0 成功，其他，错误
*/
int  torm_meta_set_name(TORM_META* meta, const char* name);

/**
* 设置操作元数据所有项,执行此操作后,表示要对该元数据的所有项进行操作,即记录全部字段操作
*@return, 0 成功，其他，错误
*/
int  torm_meta_set_all_entrys (TORM_META* meta);

/**
* 设置操作元数据的操作项,对记录进行部分字段操作
*@return, 0 成功，其他，错误
*/
int  torm_meta_set_entrys (TORM_META* meta, char* entrys[], int entry_num);

/**
* 反选元数据操作项,和torm_meta_set_entrys作用相反,对记录除去设置的字段外的其他字段进行操作
*@return, 0 成功，其他，错误
*/
int  torm_meta_unset_entrys (TORM *torm, TORM_META* meta, char* entrys[], int entry_num);

/**
* 根据类型初始化操作,相当于构造函数
*@param[in]    operate_type 操作类型，bind to TORM_OPERATE_TYPE
*@param[in]    operate 操作指针
*@return, 0 成功，其他，错误
*/
int  torm_operate_init(int operate_type, TORM_OPERATE *operate);

/**
* 初始化torm句柄，相当于构造函数.根据torm默认参数初始化，是执行其他初始化操作的前提条件
*@param[in,out]    torm 句柄 
*@return, 0 成功，其他，错误
*/
int  torm_init(TORM *torm);

/**
* 初始化torm 元数据库，需要在torm_init 后执行
*@note 不支持windows下使用，windows下请使用torm_create_handle_files
*@param[in,out]    torm 句柄 
*@param[in]    meta_base_key 元数据共享内存key
*@param[in]    meta_lib  元数据库名称
*@param[in]    lib_count 元数据库数目
*@return, 0 成功，其他，错误
*/
int  torm_init_lib(TORM *torm, const char* meta_base_key, char* meta_lib[], int lib_count);

/**
* 从文件初始化torm 元数据库，需要在torm_init 后执行
*@param[in,out]    torm 句柄 
*@param[in]    lib_files  元数据文件名数组
*@param[in]    lib_count 元数据文件数目
*@return, 0 成功，其他，错误
*/
int  torm_init_lib_files(TORM *torm, char* lib_files[], int file_count);

/**
* 初始化通信bus，需要在torm_init 后执行
*@param[in]    torm 句柄 
*@param[in]    src 本进程地址
*@param[in]     dest 服务器进程地址
*@return, 0 成功，其他，错误
*/
int  torm_init_bus(TORM *torm, const char* src, const char* dest);

/**
* 反初始化torm,相当于析构函数
*/
int  torm_uninit(TORM *torm);

/**
* 注册回调函数,torm需要将要执行的所有操作的回调函数进行注册，需要在torm_init后执行
*@param[in,out]    torm 句柄 
*@param[in]    Id 回调函数id, 范围支持1-1023
*@param[in]   func 回调函数地址
*@return, 0 成功，其他，错误
*/
int  torm_add_callback_func (TORM *torm, unsigned int Id, RES_FUNC func);

/**
* 设置torm 默认回调函数，如果某个操作没有指定回调函数,则执行默认回调函数,需要在torm_init后执行
*@param[in,out]    torm 句柄 
*@param[in]    Id 回调函数id
*@return, 0 成功，其他，错误
*/
int  torm_set_response_handle (TORM* torm, int Id);

/**
* torm执行插入操作
*@param[in] torm torm实例句柄
*@param[in] operate  torm操作
*@param[in] data  操作对象
*@param[in] len 对象大小
*@param[in] async_data  异步数据，在结果返回回调的时候带回
*@param[in] async_len 异步数据大小,最大支持1024
*@return 0 成功; 非零，失败
*/
int  torm_operate_insert (TORM* torm, TORM_OPERATE_INSERT *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* torm执行更新操作
*@param[in] torm torm实例句柄
*@param[in] operate  torm操作
*@param[in] data  操作对象
*@param[in] len 对象大小
*@param[in] async_data  异步数据，在结果返回回调的时候带回
*@param[in] async_len 异步数据大小,最大支持1024
*@return 0 成功; 非零，失败
*/
int  torm_operate_update (TORM* torm, TORM_OPERATE_UPDATE *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* torm执行删除操作
*@param[in] torm torm实例句柄
*@param[in] operate  torm操作
*@param[in] data  操作对象
*@param[in] len 对象大小
*@param[in] async_data  异步数据，在结果返回回调的时候带回
*@param[in] async_len 异步数据大小,最大支持1024
*@return 0 成功; 非零，失败
*/
int  torm_operate_delete (TORM* torm, TORM_OPERATE_DELETE *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* torm执行读取操作
*@param[in] torm torm实例句柄
*@param[in] operate  torm操作
*@param[in] data  操作对象
*@param[in] len 对象大小
*@param[in] async_data  异步数据，在结果返回回调的时候带回
*@param[in] async_len 异步数据大小,最大支持1024
*@return 0 成功; 非零，失败
*/
int  torm_operate_select (TORM* torm, TORM_OPERATE_SELECT *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* torm执行查询记录行数操作
*@param[in] torm torm实例句柄
*@param[in] operate  torm操作
*@param[in] data  操作对象
*@param[in] len 对象大小
*@param[in] async_data  异步数据，在结果返回回调的时候带回
*@param[in] async_len 异步数据大小,最大支持1024
*@return 0 成功; 非零，失败
*/
int  torm_operate_select_count (TORM* torm, TORM_OPERATE_SELECT_COUNT *operate, 
                                    char* data, int len, char *async_data, int async_len);

/**
* torm 从指定的数据表查询，tormsvr在多线程模式并且操作的元数据使用cache的情况下不支持该操作
*@param[in] torm torm实例句柄
*@param[in] operate  torm操作
*@param[in] data  操作对象
*@param[in] len 对象大小
*@param[in] async_data  异步数据，在结果返回回调的时候带回
*@param[in] async_len 异步数据大小,最大支持1024
*@return 0 成功; 非零，失败
*/
int  torm_operate_select_table (TORM* torm, TORM_OPERATE_SELECT_TABLE *operate, 
                                    char* data, int len, char *async_data, int async_len);


/**
* torm执行查询记录行数操作
*@param[in] torm torm实例句柄
*@param[in] operate  torm操作
*@param[in] svr_id 查询server的id，在通过proxy访问时需要指定
*@param[in] async_data  异步数据，在结果返回回调的时候带回
*@param[in] async_len 异步数据大小,最大支持1024
*@return 0 成功; 非零，失败
*/
int  torm_operate_svrinfo (TORM* torm, TORM_OPERATE_SVRINFO *operate, 
                                    int svr_id, char *async_data, int async_len);

/**
* 触发torm收取和处理返回消息
*@param[in] torm torm实例句柄
*@param[in] max_msg_count 该次调用最大处理消息数
*@return 实际处理消息数
*/
int  torm_do_recv (TORM* torm, int max_msg_count);

/**
* 只对TORM_OPERATE_TYPE_UPDATE和TORM_OPERATE_TYPE_DELETE的操作有效，
* 获得操作影响的记录数目
*@param[in] torm torm实例句柄
*@return   影响的记录数目
*/
int  torm_affected_rows(TORM *torm);

/**
* 只对当前操作为TORM_OPERATE_TYPE_SELECT类型的操作有效，
* 获得操作结果的总记录条数，当前返回记录条数和起始点
*@param[in] torm torm实例句柄
*@return 0 成功; 非零，失败
*@param[in,out] total_num 记录总数
*@param[in,out] curr_num 当前返回记录数
*@param[in,out] offset 当前返回记录偏移点
*/
int  torm_num_rows(TORM *torm, int* total_num, int* curr_num, int* offset);

/**
* 只对当前操作为TORM_OPERATE_TYPE_SELECT类型的操作有效
*@param[in] torm torm实例句柄
*@param[in,out] buff 记录结果
*@param[in,out] buff_len : 空间大小
*@return 0 成功; 非零，失败
*/
int  torm_fetch_record(TORM *torm, char* buff, int buff_len);

/**
* 只对当前操作为TORM_OPERATE_TYPE_INSERT 操作有效
*@param[in] torm torm实例句柄
*@return 最后插入的id
*/
tdr_ulonglong torm_last_insert_id(TORM* torm);

/**
* 只对当前操作为TORM_OPERATE_TYPE_SELECT_COUNT 操作有效
*@param[in] torm torm实例句柄
*@return 返回行数
*/
int torm_select_count(TORM* torm);

/**
* 只对当前操作为TORM_OPERATE_TYPE_SVRINFO 操作有效
*@param[in] torm torm实例句柄
*@return 服务器信息
*/
TORMSVRINFORES* torm_svrinfo(TORM* torm);

/**
* 获得torm的执行的错误号码
*/
int  torm_errno(TORM *torm);


/**
* 获得torm 号码对应的错误信息
*/
const char* torm_errstr(int err);


/**
* 获得torm的执行的数据库错误号
*/
int  torm_dbms_errno(TORM *torm);


/**
* 获得torm的当前操作,在处理torm结果的时候，可以通过该接口获得当前操作的具体信息，注意，操作的res_flag的值会对操作的信息有一定的影响。
*/
const TORM_OPERATE*  torm_get_operate(TORM *torm);


/**
* 设置torm的日志句柄,必须在调用torm_init_lib成功之后调用
*/
void torm_set_log(TORM* torm, LPTLOGCATEGORYINST a_pstTormapiCat);


/**@} */ /*  defgroup  TORM TORM_协议和bus封装API */


/**
* 临时接口
*/
//int torm_set_bus_handle(TORM* torm, int bus_handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TORM_API_WRAPPER_H */
