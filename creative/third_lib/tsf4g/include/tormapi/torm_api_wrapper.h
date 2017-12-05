

/**
*
* @file     torm_api_wrapper.h
* @brief    ��torm �ײ�ӿں�bus�Ļ����Ͻ��з�װ,�ṩһ�׸��ϲ�� 
* Ӧ�ó���ӿ�,����Ӧ�ó����˽�torm�ڲ�Э��ϸ�ں�bus�ӿ�ϸ��,
* ���ϵͳ�Ŀ���չ��.
* 
* @author vinsonzuo
* @version 1.0
* @date 2009-10-16 
*
*
* Copyright (c)  2009, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/



#ifndef TORM_API_WRAPPER_H
#define TORM_API_WRAPPER_H

#include "tormapi/torm_protocol.h"
#include "tormapi/torm_protocol_api.h"
//#include "tormapi/torm_log.h"

#define TORM_MIN_CALLBACK_ID 1	/* ע��torm �ص�����id ��Сֵ*/
#define TORM_MAX_CALLBACK_ID 1023  /* ע��torm �ص�����id ���ֵ*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
* torm ��ز�������
*/
enum TORM_OPTION_NAME
{
    TORM_OPT_NO, /* ��Ч���� */
    TORM_OPT_MAX_NET_BUFF_LEN, /* ��Ϣ��󳤶ȣ�Ĭ��65536*/
    TORM_OPT_RUN_MODE, /* ����ģʽ��Ŀǰֻ֧���첽��Ĭ���첽*/
    TORM_OPT_CONN_MODE, /* ����ģʽ��Ŀǰ֧������proxy��ֱ��tormsvr*/
};
    
/****************************************/
/**
* torm ��������
*/
enum TORM_OPERATE_TYPE
{
    TORM_OPERATE_TYPE_NO,
    TORM_OPERATE_TYPE_INSERT, /* ��Ӧ���ݿ�insert���� */
    TORM_OPERATE_TYPE_UPDATE, /* ��Ӧ���ݿ�update���� */
    TORM_OPERATE_TYPE_SELECT, /* ��Ӧ���ݿ�select���� */
    TORM_OPERATE_TYPE_DELETE, /* ��Ӧ���ݿ�delete���� */
    TORM_OPERATE_TYPE_SELECT_COUNT, /* ���������Ļ�ü�¼�����Ĳ��� */
    TORM_OPERATE_TYPE_SVRINFO, /* ���tormsvr�������״̬��Ϣ�Ĳ��� */
    TORM_OPERATE_TYPE_SELECT_TABLE, /* ָ�������Ʋ�ѯ*/
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
* torm �����ṹ
*/
typedef struct st_torm_options 
{
    /**
    * ����ģʽ��
    * 1: ͬ��ģʽ��Ŀǰ��֧��
    * 0: �첽ģʽ���ڸ�ģʽ�£��ڷ���������Ϣ�����̷��أ�
    *   ��torm_do_recvʱ���鷵�����
    */
    unsigned int run_mode;
    
    
    /** ����ģʽ����������proxy��ֱ��tormsvr
    * 1: ����proxy��ת�����Զ���tormЭ���ǰ����proxy��ͷ
    * 0: ֱ������tormsvr
    */
    unsigned int conn_mode;
    
    /**
    * ������Ϣ������
    * > 0��������
    * ����: ʹ��Ĭ��ֵTORM_MAX_PKG_LEN
    */           
    unsigned int max_net_buff_len;
    
}TORM_OPTION ;

/**
* torm ����Ԫ���ݶ���
*/
typedef struct st_torm_meta
{
    char name[TORM_MAX_META_NAME];   /* Ԫ�������� */
    unsigned int verion;    /* Ԫ���ݰ汾*/
    unsigned int entry_num;  /* ��������Ŀ*/                 
    char entrys[TORM_MAX_ENTRYLIST][TORM_MAX_META_NAME]; /* ������*/
}TORM_META; 


/**
* torm �������ػ������
*/
struct st_torm_result
{
   int operate_type;   /* �������ͣ�bind to TORM_OPERATE_TYPE */
   int result;    /* ������� */
};

/**
* torm �������
*/
struct st_torm_operate_insert
{
    TORM_META  meta;   /*������Ե�Ԫ����*/
    int callback_id;  /*�������ش�����*/
    unsigned int res_flag;
};

/**
* torm ���²���
*/
struct st_torm_operate_update
{
    TORM_META  meta;   /* ������Ե�Ԫ���� */ 
    char  where[TORM_MAX_WHERE_DEF] ;   /* where���������û�У�����Ϊ���ַ���*/
    int do_insert;    /* �Ƿ��ڸ��¼�¼�����ڵ�����²����µļ�¼*/
    int callback_id;  /*�������ش�����*/
    unsigned int res_flag;
};

/**
* torm ɾ������
*/
struct st_torm_operate_delete
{
    TORM_META  meta;   /* ������Ե�Ԫ���� */ 
    char where[TORM_MAX_WHERE_DEF];  /* ���û���Զ���������������Ϊ���ַ��� */
    int callback_id;  /*�������ش�����*/
    unsigned int res_flag;
};

/**
* torm ��ȡ����
*/
struct st_torm_operate_select
{
    TORM_META  meta;   /* ������Ե�Ԫ���� */ 
    char where[TORM_MAX_WHERE_DEF];   /* ���û���Զ���������������Ϊ���ַ��� */
    int offset;       /* ���ڵ�������Ч */
    int limit;        /* ��������Ч*/
    int callback_id;  /*�������ش�����*/
    unsigned int res_flag;
};

/**
* torm ��ָ�������ݱ��ȡ����
*/
struct st_torm_operate_select_table
{
    TORM_META  meta;   /* ������Ե�Ԫ���� */ 
	char table_name[TORM_MAX_TABLE_NAME];  /*���ݱ�����*/
	int insert_cache;  /*�Ƿ񽫲�ѯ������ص�cache*/
    char where[TORM_MAX_WHERE_DEF];   /* ���û���Զ���������������Ϊ���ַ��� */
    int offset;       /* ���ڵ�������Ч */
    int limit;        /* ��������Ч*/
    int callback_id;  /*�������ش�����*/
    unsigned int res_flag;
};


/**
* torm ���Ҽ�¼����������
*/
struct st_torm_operate_select_count
{
    TORM_META  meta;   /* ������Ե�Ԫ���� */ 
    char where[TORM_MAX_WHERE_DEF];   /* ���û���Զ���������������Ϊ���ַ��� */
    int callback_id;  /*�������ش�����*/
};

/**
* torm ��ѯ�������״̬�Ĳ���
*/
struct st_torm_operate_svrinfo
{
    char name[TORM_MAX_META_NAME];   /* Ԫ�������� */
    int callback_id;  /*�������ش�����*/
};

/**
* torm ����
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
* ���ش�����
*@param[in]    torm torm���
*@param[in]    async_data �첽���ݣ���ִ�в���ʱ��������ݷ���
*@param[in]    async_len �첽���ݳ��ȣ��1024
*@param[out]    result �����������
*@return, 0: ����ɹ�
        ����: ����ʧ��
*/
typedef int (*RES_FUNC) (TORM* torm, char *async_data, int async_len, TORM_RESULT* result);

/******************************************/
/**
* torm������ϸ������ڲ����ݽṹ
*/
typedef struct st_torm_result_in
{
    TORMPKG res_pkg; /* �������ذ� */

    TORM_OPERATE operate; /* �ӷ��ؽ�����ع�������Ϣ*/
} TORM_RESULT_IN ;

/******************************************/
/**
* torm�ص������б��ڲ����ݽṹ
*/
typedef struct st_torm_callback_mgr
{
    RES_FUNC func_list[TORM_MAX_CALLBACK_ID+1]; /* �����±�Ϊ��Ӧ�Ļص�����id*/
    
} TORM_CALLBACK_MGR ;


/*********************************************/
/**
* torm �����ݽṹ���ڲ����ݽṹ
*/
struct st_torm
{
    TORMPROTOCOLHANDLE proto_handle;   /* LPTORMPROHANDLER */
    int bus_handle;     /* busͨ�ž��*/

    TORMPKG req_pkg;   /* ����Э���*/

    char* net_buff;        /* ����Э��ռ��ַ*/
    int net_buff_len;    /* ����Э��ռ䳤��*/

    TORM_OPTION options;  /* torm �������� */
    
    TORM_RESULT_IN  result;   /* ��ǰ�������ؽ�� */
    
    TORM_CALLBACK_MGR callback_tab;  /* �ص��������� */

    int callback_id;  /* Ĭ�ϲ������ش����� */ 

    int proc_id;   /* �������̵�bus id*/
	int svr_id;    /*����server�Ľ���id*/
};
/**********************************************/

/**@defgroup  TORM  TORM_Э���bus��װAPI
* @{
*@note Torm Э���bus��װAPI��˼�룺
*  - ��Ӧ������tormЭ�����ݣ�����tormЭ�����չ�Ͱ汾������Ӧ�ò�Ӧ��ֱ�Ӷ�Э����в���
*  - ��Ӧ������busͨ�Ų��֣�Ӧ��ֻ��Ҫ��ʼ��ʱ��������̺ź�tormsvr���̺Ž������Ӽ���
*  - ��Ӧ��������Ϊ��ĳ��Ӧ�����ݽ���ĳ������
*  - Ӧ�õ���Ҫ�����Ƕ��������������Ҫ������Ҫ���в��������ͣ��ֶΣ����������Լ�����������غ�Ĵ�����ID��
*  - Ӧ�ö���Ĳ������õ��Ľ�����ش�������Ҫ��torm�н���ע��
*  - Ӧ��ͨ���ӿڻ�ȡ���ؽ��
*  - �ӿ�ʹ��������ο�����ĵ���ʵ������
*  - Ӧ�ö��������ʱ�����ֱ�ӶԲ����ṹ���ֶν��и�ֵ��Ϊ�˷���汾������Ӧ����ʹ��torm_operate_init����ʼ������
*/

/**
* ���ò���ֵ,֧�ֵĲ���������ο�TORM_OPTION_NAME
*@param[in]    torm torm���
*@param[in]    opt_name ������
*@param[in]    opt_val �������ݵ�ַ
*@param[in]      opt_len �������ݳ���
*@return, 0 �ɹ�������������
*/
int  torm_options(TORM *torm, int opt_name, void* opt_val, int opt_len);

/**
* ���ò���ԱԪ���ݵ�����
*@param[in]    meta meta���
*@param[in]    name Ԫ��������
*@return, 0 �ɹ�������������
*/
int  torm_meta_set_name(TORM_META* meta, const char* name);

/**
* ���ò���Ԫ����������,ִ�д˲�����,��ʾҪ�Ը�Ԫ���ݵ���������в���,����¼ȫ���ֶβ���
*@return, 0 �ɹ�������������
*/
int  torm_meta_set_all_entrys (TORM_META* meta);

/**
* ���ò���Ԫ���ݵĲ�����,�Լ�¼���в����ֶβ���
*@return, 0 �ɹ�������������
*/
int  torm_meta_set_entrys (TORM_META* meta, char* entrys[], int entry_num);

/**
* ��ѡԪ���ݲ�����,��torm_meta_set_entrys�����෴,�Լ�¼��ȥ���õ��ֶ���������ֶν��в���
*@return, 0 �ɹ�������������
*/
int  torm_meta_unset_entrys (TORM *torm, TORM_META* meta, char* entrys[], int entry_num);

/**
* �������ͳ�ʼ������,�൱�ڹ��캯��
*@param[in]    operate_type �������ͣ�bind to TORM_OPERATE_TYPE
*@param[in]    operate ����ָ��
*@return, 0 �ɹ�������������
*/
int  torm_operate_init(int operate_type, TORM_OPERATE *operate);

/**
* ��ʼ��torm������൱�ڹ��캯��.����tormĬ�ϲ�����ʼ������ִ��������ʼ��������ǰ������
*@param[in,out]    torm ��� 
*@return, 0 �ɹ�������������
*/
int  torm_init(TORM *torm);

/**
* ��ʼ��torm Ԫ���ݿ⣬��Ҫ��torm_init ��ִ��
*@note ��֧��windows��ʹ�ã�windows����ʹ��torm_create_handle_files
*@param[in,out]    torm ��� 
*@param[in]    meta_base_key Ԫ���ݹ����ڴ�key
*@param[in]    meta_lib  Ԫ���ݿ�����
*@param[in]    lib_count Ԫ���ݿ���Ŀ
*@return, 0 �ɹ�������������
*/
int  torm_init_lib(TORM *torm, const char* meta_base_key, char* meta_lib[], int lib_count);

/**
* ���ļ���ʼ��torm Ԫ���ݿ⣬��Ҫ��torm_init ��ִ��
*@param[in,out]    torm ��� 
*@param[in]    lib_files  Ԫ�����ļ�������
*@param[in]    lib_count Ԫ�����ļ���Ŀ
*@return, 0 �ɹ�������������
*/
int  torm_init_lib_files(TORM *torm, char* lib_files[], int file_count);

/**
* ��ʼ��ͨ��bus����Ҫ��torm_init ��ִ��
*@param[in]    torm ��� 
*@param[in]    src �����̵�ַ
*@param[in]     dest ���������̵�ַ
*@return, 0 �ɹ�������������
*/
int  torm_init_bus(TORM *torm, const char* src, const char* dest);

/**
* ����ʼ��torm,�൱����������
*/
int  torm_uninit(TORM *torm);

/**
* ע��ص�����,torm��Ҫ��Ҫִ�е����в����Ļص���������ע�ᣬ��Ҫ��torm_init��ִ��
*@param[in,out]    torm ��� 
*@param[in]    Id �ص�����id, ��Χ֧��1-1023
*@param[in]   func �ص�������ַ
*@return, 0 �ɹ�������������
*/
int  torm_add_callback_func (TORM *torm, unsigned int Id, RES_FUNC func);

/**
* ����torm Ĭ�ϻص����������ĳ������û��ָ���ص�����,��ִ��Ĭ�ϻص�����,��Ҫ��torm_init��ִ��
*@param[in,out]    torm ��� 
*@param[in]    Id �ص�����id
*@return, 0 �ɹ�������������
*/
int  torm_set_response_handle (TORM* torm, int Id);

/**
* tormִ�в������
*@param[in] torm tormʵ�����
*@param[in] operate  torm����
*@param[in] data  ��������
*@param[in] len �����С
*@param[in] async_data  �첽���ݣ��ڽ�����ػص���ʱ�����
*@param[in] async_len �첽���ݴ�С,���֧��1024
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_operate_insert (TORM* torm, TORM_OPERATE_INSERT *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* tormִ�и��²���
*@param[in] torm tormʵ�����
*@param[in] operate  torm����
*@param[in] data  ��������
*@param[in] len �����С
*@param[in] async_data  �첽���ݣ��ڽ�����ػص���ʱ�����
*@param[in] async_len �첽���ݴ�С,���֧��1024
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_operate_update (TORM* torm, TORM_OPERATE_UPDATE *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* tormִ��ɾ������
*@param[in] torm tormʵ�����
*@param[in] operate  torm����
*@param[in] data  ��������
*@param[in] len �����С
*@param[in] async_data  �첽���ݣ��ڽ�����ػص���ʱ�����
*@param[in] async_len �첽���ݴ�С,���֧��1024
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_operate_delete (TORM* torm, TORM_OPERATE_DELETE *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* tormִ�ж�ȡ����
*@param[in] torm tormʵ�����
*@param[in] operate  torm����
*@param[in] data  ��������
*@param[in] len �����С
*@param[in] async_data  �첽���ݣ��ڽ�����ػص���ʱ�����
*@param[in] async_len �첽���ݴ�С,���֧��1024
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_operate_select (TORM* torm, TORM_OPERATE_SELECT *operate, 
                            char* data, int len, char *async_data, int async_len);

/**
* tormִ�в�ѯ��¼��������
*@param[in] torm tormʵ�����
*@param[in] operate  torm����
*@param[in] data  ��������
*@param[in] len �����С
*@param[in] async_data  �첽���ݣ��ڽ�����ػص���ʱ�����
*@param[in] async_len �첽���ݴ�С,���֧��1024
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_operate_select_count (TORM* torm, TORM_OPERATE_SELECT_COUNT *operate, 
                                    char* data, int len, char *async_data, int async_len);

/**
* torm ��ָ�������ݱ��ѯ��tormsvr�ڶ��߳�ģʽ���Ҳ�����Ԫ����ʹ��cache������²�֧�ָò���
*@param[in] torm tormʵ�����
*@param[in] operate  torm����
*@param[in] data  ��������
*@param[in] len �����С
*@param[in] async_data  �첽���ݣ��ڽ�����ػص���ʱ�����
*@param[in] async_len �첽���ݴ�С,���֧��1024
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_operate_select_table (TORM* torm, TORM_OPERATE_SELECT_TABLE *operate, 
                                    char* data, int len, char *async_data, int async_len);


/**
* tormִ�в�ѯ��¼��������
*@param[in] torm tormʵ�����
*@param[in] operate  torm����
*@param[in] svr_id ��ѯserver��id����ͨ��proxy����ʱ��Ҫָ��
*@param[in] async_data  �첽���ݣ��ڽ�����ػص���ʱ�����
*@param[in] async_len �첽���ݴ�С,���֧��1024
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_operate_svrinfo (TORM* torm, TORM_OPERATE_SVRINFO *operate, 
                                    int svr_id, char *async_data, int async_len);

/**
* ����torm��ȡ�ʹ�������Ϣ
*@param[in] torm tormʵ�����
*@param[in] max_msg_count �ôε����������Ϣ��
*@return ʵ�ʴ�����Ϣ��
*/
int  torm_do_recv (TORM* torm, int max_msg_count);

/**
* ֻ��TORM_OPERATE_TYPE_UPDATE��TORM_OPERATE_TYPE_DELETE�Ĳ�����Ч��
* ��ò���Ӱ��ļ�¼��Ŀ
*@param[in] torm tormʵ�����
*@return   Ӱ��ļ�¼��Ŀ
*/
int  torm_affected_rows(TORM *torm);

/**
* ֻ�Ե�ǰ����ΪTORM_OPERATE_TYPE_SELECT���͵Ĳ�����Ч��
* ��ò���������ܼ�¼��������ǰ���ؼ�¼��������ʼ��
*@param[in] torm tormʵ�����
*@return 0 �ɹ�; ���㣬ʧ��
*@param[in,out] total_num ��¼����
*@param[in,out] curr_num ��ǰ���ؼ�¼��
*@param[in,out] offset ��ǰ���ؼ�¼ƫ�Ƶ�
*/
int  torm_num_rows(TORM *torm, int* total_num, int* curr_num, int* offset);

/**
* ֻ�Ե�ǰ����ΪTORM_OPERATE_TYPE_SELECT���͵Ĳ�����Ч
*@param[in] torm tormʵ�����
*@param[in,out] buff ��¼���
*@param[in,out] buff_len : �ռ��С
*@return 0 �ɹ�; ���㣬ʧ��
*/
int  torm_fetch_record(TORM *torm, char* buff, int buff_len);

/**
* ֻ�Ե�ǰ����ΪTORM_OPERATE_TYPE_INSERT ������Ч
*@param[in] torm tormʵ�����
*@return �������id
*/
tdr_ulonglong torm_last_insert_id(TORM* torm);

/**
* ֻ�Ե�ǰ����ΪTORM_OPERATE_TYPE_SELECT_COUNT ������Ч
*@param[in] torm tormʵ�����
*@return ��������
*/
int torm_select_count(TORM* torm);

/**
* ֻ�Ե�ǰ����ΪTORM_OPERATE_TYPE_SVRINFO ������Ч
*@param[in] torm tormʵ�����
*@return ��������Ϣ
*/
TORMSVRINFORES* torm_svrinfo(TORM* torm);

/**
* ���torm��ִ�еĴ������
*/
int  torm_errno(TORM *torm);


/**
* ���torm �����Ӧ�Ĵ�����Ϣ
*/
const char* torm_errstr(int err);


/**
* ���torm��ִ�е����ݿ�����
*/
int  torm_dbms_errno(TORM *torm);


/**
* ���torm�ĵ�ǰ����,�ڴ���torm�����ʱ�򣬿���ͨ���ýӿڻ�õ�ǰ�����ľ�����Ϣ��ע�⣬������res_flag��ֵ��Բ�������Ϣ��һ����Ӱ�졣
*/
const TORM_OPERATE*  torm_get_operate(TORM *torm);


/**
* ����torm����־���,�����ڵ���torm_init_lib�ɹ�֮�����
*/
void torm_set_log(TORM* torm, LPTLOGCATEGORYINST a_pstTormapiCat);


/**@} */ /*  defgroup  TORM TORM_Э���bus��װAPI */


/**
* ��ʱ�ӿ�
*/
//int torm_set_bus_handle(TORM* torm, int bus_handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TORM_API_WRAPPER_H */
