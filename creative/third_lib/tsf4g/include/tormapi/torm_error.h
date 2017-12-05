/**
*
* @file     torm_error.h 
* @brief    TORM��������
* 
* @author jackyai  
* @version 1.0
* @date 2007-11-28 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/

#ifndef TORM_ERROR_H
#define TORM_ERROR_H

#include "comm/tmodid.h"
#include "comm/terr.h"
#include "tdr/tdr.h"

/** @defgroup TORM_ERR TORM_������
* @{
*@note TORM������Ļ���˼��:
*  - ʹ��һ���������洢���������Ϣ��������ֵ�ĺ�������:
*      -# <0 ��ʾ����������ֵΪ�������Ĵ���
*      -# 0 ��ʾ�ɹ�
*      -# >0 ��ʾ�ɹ�����������ĳЩ��������
*  - �������洢��4�ֽڵ������У������������:
*      -# ���󼶱�: 1���ֽڣ�����Ϊ��λ�ĵ�1���ֽ�
*      -# ģ��ID:  1�ֽڣ�����Ϊ��λ�ĵ�2���ֽ�
*      -# �����:  2�ֽڣ�0-1023�ű�ʾͨ�ô��󣬸�ģ�鶨������1024��ʼ
* 
*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define TORM_SUCCESS				0
#define TORM_ERR_ERROR				-1


#define TORM_ERR_MAKE_DEBUG(errno)		TERR_MAKE_DEBUG(TMODID_ORM, errno)
#define TORM_ERR_MAKE_WARN(errno)		TERR_MAKE_WARN(TMODID_ORM, errno)
#define TORM_ERR_MAKE_ERROR(errno)		TERR_MAKE_ERROR(TMODID_ORM, errno)
#define TORM_ERR_MAKE_SEVERE(errno)		TERR_MAKE_SEVERE(TMODID_ORM, errno)


#define TORM_ERR_IS_ERROR(error)			TERR_IS_ERROR(error)

/*ORM��ģ�����������*/
#define TORM_ERRNO_SVE_BASE		TERR_MAX_ERRNO	/*ORM������ʹ�õĴ���������*/
#define TORM_ERRNO_PROTOCOLAPI_BASE		TERR_MAX_ERRNO +1024	/*ORM Э�鴦��API�Ĵ���������*/
#define TORM_ERRNO_DBAPI_BASE		TERR_MAX_ERRNO +2048	/*ORM Э�鴦��API�Ĵ���������*/

/*������ϴ������*/
#define TORM_ERR_MAKE_SVR_ERROR(err)    TORM_ERR_MAKE_ERROR(TORM_ERRNO_SVE_BASE + err)
#define TORM_ERR_MAKE_PROTOCOLAPI_ERROR(err)    TORM_ERR_MAKE_ERROR(TORM_ERRNO_PROTOCOLAPI_BASE + err)
#define TORM_ERR_MAKE_DBAPI_ERROR(err)    TORM_ERR_MAKE_ERROR(TORM_ERRNO_DBAPI_BASE + err)

	/**
	*TORM SVR������붨�� 
	*/
	typedef enum
	{
		TORM_ERROR_SVR_NONE = 0,                    /**< û�д���*/

		TORM_ERROR_CHANNEL_CONFUSE,
		TORM_ERROR_CHANNEL_BADMSG,
		TORM_ERROR_CHANNEL_FULL,
		TORM_ERROR_CHANNEL_EMPTY,
		TORM_ERROR_CHANNEL_ROUND,
		TORM_ERROR_CHANNEL_NOENOUGH,
		TORM_ERROR_QUERY,
		TORM_ERROR_QUERY_DUPKEY,
		TORM_ERROR_PART_PRIKEY,
		TORM_ERROR_CONDITION_INSERT,		/*��֧��where�����Ĳ������*/
		
		TORM_ERROR_SVR_INVALID_META,		/*��Ч�Ĳ�����������*/
		TORM_ERROR_SVR_GENERATE_SQL,		/*�������ݲ���SQLʧ��*/
		TORM_ERROR_SVR_PACK_RECORD,			/*���������������ʱʧ��*/
		TORM_ERROR_SVR_CACHE_OPE,           /*�����������*/

		TORM_ERROR_SVR_BUSY,           		/*����æ*/
		
		TORM_ERROR_SVR_COUNT                     /**< ������������־*/ 
	} TORMSVRERROR;

	/**
	*TORM Protocolapi������붨�� 
	*/
	typedef enum
	{
		TORM_ERROR_PROTOCOLAPI_NONE = 0,                    /**< û�д���*/
		TORM_ERROR_PROTOCOLAPI_NOMEM,				/**<�����ڴ�ʧ��*/
		TORM_ERROR_PROTOCOLAPI_OPENMB,				/**<�򿪹������ڴ���Ԫ���ݿ⼯�ϵľ��ʧ��*/
		TORM_ERROR_PROTOCOLAPI_UNFINDMETALIB,       /**<�ڹ����ڴ���Ԫ���ݿ⼯�ϲ���ָ��Ԫ���ݿ�ʧ��*/
		TORM_ERROR_PTOTOCOLAPI_UNFINDMETA,			/**<��Ԫ�������������Ҳ���ָ�����ݵ�Ԫ��������*/
		TORM_ERROR_PROTOCOLAPI_UNSUPPORTCMD,		/**<��Ϣ�е�CmdID�в�֧��*/
		TORM_ERROR_PROTOCOLAPI_NOPRIMRYKEY,			/**<�������ݽṹ����ȱ�����ݿ���������Ϣ*/
		TORM_ERROR_PROTOCOLAPI_METANAMECONFLICT,		/**<�������ݵ�Ԫ�����������е�Ԫ������ͬ��,Ŀǰ��֧��*/
		TORM_ERROR_PROTOCOLAPI_METAINDEXNUMCONFLICT,	/**<�������ݵ�Ԫ����������ʵ����������Ԥ�ڵĲ�һ��*/
		TORM_ERROR_PROTOCOLAPI_INVALID_PROTOCOL_CUT_VER,	/**<Э��ļ��ð汾��Э��Ԫ���ݵĻ��汾��С*/
		TORM_ERROR_PROTOCOLAPI_DATA_CUT_VER,			/**<�������ݵļ��ð汾����Ԫ���ݵĻ��汾��С*/
		TORM_ERROR_PRO_NO_PACK_BUFF,					/**<���ݴ��ʱ�洢�ռ䲻��*/
		TORM_ERROR_PRO_INVALID_DATA_BUFF,				/**<�洢�������ݵĻ�������Ч:��ָ���ռ�С��Ԥ��*/
		TORM_ERROR_PRO_UNFINDENTRYOFFINDEX,				/**<�ڳ�Աƫ�����������Ҳ���ָ����Ա��ƫ��*/
		TORM_ERROR_PRO_INSUFFICIENT_DATA_BUFF,			/**<�������ݻ�����С��Ԥ�ڵĴ�С*/
		TORM_ERROR_PRO_NOSPLITENTRY,					/**<�����ݶ�Ӧ�����ݿ���Ƿֱ�洢��,��ָ���˷ֱ��Ա,���˲�������Ԫ����������û��ָ���ֱ��Ա*/
		TORM_ERROR_PRO_CUTOFF_SPLITENTRY,				/**<�����ݶ�Ӧ�����ݿ���Ƿֱ�洢��,��ָ���˷ֱ��Ա,���ð汾���ֱ��Ա���õ���*/
		TORM_ERROR_PRO_UNFINEDENTRYNAMEINDEX,			/**<�ڳ�Ա����-ָ���������и��������Ҳ����˳�Ա����Ϣ*/
		TORM_ERROR_PRO_NAMELISTLIMITED,					/**<��Ҫ�ڲ�����Ա�б��м��������Ա,�����б��Ѿ��ﵽ������Ա������*/
		TORM_ERROR_PRO_FAILEDCALCADDR,					/**<ͨ��Ԫ����������λ��Ա�ĵ�ַʧ��*/
		TORM_ERROR_PRO_INVALIDREFERVALUE,				/**<��Ա��refer���Բ���ȷ*/
		TORM_ERROR_PRO_NODATA,							/**<�Ѿ�û�����ݿ�ȡ*/
		TORM_ERROR_PRO_NOUNPACKBUFF,					/**<����������ݻ�������С��Ԥ�ڵ�ҪС*/
		TORM_ERROR_PRO_INVALIDSTRINGLEN,				/**<string������Ϣ�ĳ�����Ϣ����ȷ*/
		TORM_ERROR_PRO_INVALIDPKGLEN,					/**<ORMЭ����Ϣ����У��ʧ��*/
		TORM_ERROR_PRO_PRIMARYKEYNUMLIMITED,			/**<���������Ϣ�ĳ�Ա�������������Ŀ����*/
		TORM_ERROR_PRO_NOSQLBUFF,						/**<����SQL���Ļ������ռ䲻��*/
		TORM_ERROR_PRO_DATA_VERSION_TOO_HIGHT,			/**<�������ݵļ��ð汾���ܱ�Ԫ����������İ汾��*/
		TORM_ERROR_PRO_NOTNULLLIMIT,					/**<�˲������ݶ�Ӧ�����ݿ�����NOT NULL��Ա,���Ǹ���ָ���汾������ʱ,��������NOT NULL��Ա*/
		TORM_ERROR_PRO_INVALIDSELECTOR,					/**<���ݳ�Ա��select������ָ������ֵ����ȷ*/
		TORM_ERROR_PRO_EXTERN_COMPOSITE_ARRAY,			/**<���Խṹ��չ���洢�����ݿ���ʱ���������ӳ�Ա�����ֲ�Ψһ���ᵼ�����ɵ����ݿ��������ͻ*/
		TORM_ERROR_PRO_SAVE_RECORDS,					/**<����select��Ӧ�м����������ݵ������ʱʧ��*/
		TORM_ERROR_PRO_INVALID_PARAM,					/**<���ݸ��ӿڵĲ�����Ч*/
		TORM_ERROR_PRO_ADD_LIMITS_INFO,					/**<���Զ����ѯ����������LIMIT��Ϣʱʧ��*/

		TORM_ERROR_APIWRAPPER_INVALID_PARAM,
		TORM_ERROR_APIWRAPPER_ERROR_CALLBACK_ID,
		TORM_ERROR_APIWRAPPER_ERROR_CMD,
		TORM_ERROR_APIWRAPPER_RESULT_TYPE,
		TORM_ERROR_APIWRAPPER_SYSTEM_ERROR,
		TORM_ERROR_APIWRAPPER_BUS_ERROR,

        TORM_ERROR_PRO_NO_MORE_ENTRY,
        TORM_ERROR_API_NOT_SUPPORT_WINDOWS,             /**<��api��֧��windows*/

		TORM_ERROR_PROTOCOLAPI_COUNT                     /**< ������������־*/ 		
	} TORMPROTOCOLERROR;

	/**
	*TORM db api������붨�� 
	*/
	typedef enum
	{
		TORM_ERROR_DBAPI_NONE = 0,                    /**< û�д���*/
		TORM_ERROR_DBAPI_COUNT                     /**< ������������־*/ 
	} TORMDBERROR;


	/**
	* ���ݴ�������ȡ������Ϣ
	* @param[in] iErrorCode �������
	*
	* @return  ������Ϣ����ָ��
	*/
	extern char const*	torm_error_string(IN int iErrorCode);

	/** @} */ // TDR_ERR TDR������

#ifdef __cplusplus
}
#endif




#endif /* TORM_ERROR_H */
