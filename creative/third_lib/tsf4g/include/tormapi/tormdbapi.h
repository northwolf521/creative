/**
*
* @file     tormdbapi.h  
* @brief    TORM���������̨���ݿ����ϵͳ֮�佻��API�ӿ�ͷ�ļ�
*	�˽ӿ�����TDR ��tdr_sql.h�ж���ӿڵ����䣬TORM���������tdr_sql.h�еĽӿںͱ��ļ��ж���
*   �Ľӿ�������̨���ݿ����ϵͳ֮�佻��
* 
* @author jackyai  
* @version 1.0
* @date 2007-11-23 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/

#ifndef TORMAPIDBAPI_H
#define TORMAPIDBAPI_H


#include "tormapi/torm_error.h"
#include "tormapi/torm_protocol.h"
#include "tormapi/torm_protocol_api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct  tagTormDBObject
{
	int iVersion;		/**<�������ݵİ汾*/
	LPTDRMETA	pstMeta; /**<�������ݵ�Ԫ�����������*/
	LPTORMOPERINFO pstOperInfo; /**<��������*/
};
typedef struct  tagTormDBObject TORMDBOBJECT;
typedef struct  tagTormDBObject	*LPTORMDBOBJECT;


int torm_get_table_idx(IN LPTORMSLIGHTPACK a_pstSplitInfo, IN LPTDRMETA a_pstMeta, int* a_piIdx);


	/** @defgroup TORM_DBAPI TORM_�����ϵӳ��
	* @{
	*/

/**�������ݲ�����Ϣ����SQL������� 
	*@param[out] a_pszNameBuff ������������������ݿ�����Ļ�����
	*@param[in] a_iNameBuffSize �������������ÿռ�	
	*@param[in] a_pstSplitInfo ����������ݵķֱ���Ϣ
	*@param[in] a_pstMeta	�������ݵ�Ԫ�����������
	*
	* @pre \e a_pszNameBuff ����ΪNULL
	* @pre \e a_pstOperInfo ����ΪNULL
	* @pre \e a_pstMeta ����ΪNULL
	*
	*@retval 0   ����ɹ�
	*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*/
int torm_get_table_name(OUT char *a_pszNameBuff, IN int a_iNameBuffSize, IN LPTORMSLIGHTPACK a_pstSplitInfo, IN LPTDRMETA a_pstMeta);


	
/**�������ݲ�����Ϣ����SQL������� 
*@param[in] a_hOrmHandler	ORMЭ�鴦����
*@param[in] a_hDBHandler ����DBMS������
*@param[in,out] a_pstSql ����SQL���Ļ������ṹָ��
*	- ����	a_pstSql.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstSql.iBuff	ָ���������Ĵ�С
*	- ���  a_pstSql.iBuff  ʵ��������Ϣ���ܳ���
*@param[in] a_iOPType DB�����ģ�Insert��Update��Delete��Select��ID,����ֵ�Ķ�����ο�tdr_define.h��TDRDBOPID����
*@param[in,out] a_pstObject �������������Ϣ�ṹ
*	-	a_pstObject.pstMeta �������ݵ�Ԫ�����������
*	-	a_pstObject.iVersion	 �������ݵİ汾�������ֵΪ0������߰汾����
*	-	a_pstObject.pstOperInfo	����������Ϣ
*	-	a_pstObject.pstMeta ��ָ����ܻ᲻��д
*@param[in] a_pszTable ָ�����ݱ�����,ֻ����ָ�����ѯ��ʱ����Ҫ����,������������NULL����
*
* @pre \e a_hDBHandle ������ͨ��tdr_open_dbhanlde�򿪵ľ��
* @pre \e a_pstSql ����Ϊ NULL
* @pre \e a_pstObject ����ΪNULL
* @pre \e a_pstMeta ����ΪNULL
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*/
int torm_operinfo2sql(IN TORMPROTOCOLHANDLE a_hOrmHandler, IN TDRDBHANDLE a_hDBHandler, INOUT LPTDRDATA a_pstSql, IN int a_iOPType, INOUT LPTORMDBOBJECT a_pstObject, IN char* a_pszTable );


/** ����Ԫ���������������������һ�����ݣ�������ָ�����ð汾�����ݰ�������Ϣ��ʽ�����ָ����������
*@param[in] a_hOrmHandler	ORMЭ�鴦����
*@param[in] a_hDBResult ����DB����������ľ��
*@param[in,out] a_pstRecord �������ݵĻ�������Ϣ
*	- ����	a_pstRecord.SubPack ָ�������ݵĻ������׵�ַ
*	- ����  a_pstRecord.SubPackLen	�������������ֽ���
*	- ���  a_pstRecord.iBuff  ��ʵ�ʴ�������ֽ���
*@param[in] a_pstDBObj �������ݽṹ��ָ��
*@param[in] a_bDoGetPrikey ָʾ�Ƿ�ȡ��������Ϣ
*
* @pre \e a_hDBResult ������ͨ��tdr_query��tdr_query_quick��ȡ�Ľ�����
* @pre \e a_pstRecord ����ΪNULL
* @pre \e a_pstDBObj ����ΪNULL
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ���������
*	- TDR_SUCWARN_TRUNCATE_DATE ��������ʱ���������ݽض�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*/
int  torm_fetch_row(IN TORMPROTOCOLHANDLE a_hOrmHandler, IN TDRDBRESULTHANDLE a_hDBResult,INOUT LPTORMSUBPACK a_pstRecord, 
					IN LPTORMDBOBJECT a_pstDBObj, IN TDRBOOLEAN a_bDoGetPrikey);


/**�������ݲ�����Ϣ����SQL������� 
*@param[in] a_hOrmHandler	ORMЭ�鴦����
*@param[in,out] a_pstRecord ����������ݵĻ������ṹָ��
*	- ����	a_pstRecord.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstRecord.iBuff	ָ���������Ĵ�С
*	- ���  a_pstRecord.iBuff  ʵ��������Ϣ���ܳ���
*@param[in] a_pstOperInfo	����������Ϣ
*@param[in] a_pstMeta �������ݵ�Ԫ�����������
*@param[in] a_iVersion	 �������ݵİ汾�������ֵΪ0������߰汾����
*
*
* @pre \e a_hOrmHandler ������ͨ��tdr_open_dbhanlde�򿪵ľ��
* @pre \e a_pstHost ����Ϊ NULL
* @pre \e a_pstObject ����ΪNULL
* @pre \e a_pstMeta ����ΪNULL
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*/
int torm_update_record(IN TORMPROTOCOLHANDLE a_hOrmHandler, INOUT LPTDRDATA a_pstRecord, IN LPTORMOPERINFO a_pstOperInfo, IN LPTDRMETA a_pstMeta, IN int a_iVersion);


/**���ղ�����Ա�б����������� 
*@param[in] a_hOrmHandler	ORMЭ�鴦����
*@param[in,out] a_pstOrmPack ���������ݵĽṹ֧��
*	- ���  a_pstOrmPack.SubPackLen  ʵ��������Ϣ���ܳ���
*@param[in] a_pstRecord ����������ݵĻ������ṹָ��
*	- ����	a_pstRecord.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstRecord.iBuff	ָ���������Ĵ�С
*@param[in] a_pstEntryList �ṹ�����ݳ�Ա�б�����б�Ϊ�գ����ʾȡ�����г�Ա������
*@param[in]	a_pstMeta �������ݵ�Ԫ�����������
*@param[in]	a_iVersion	 �������ݵİ汾�������ֵΪ0������߰汾����
*
* @pre \e a_hOrmHandler ������ͨ��tdr_open_dbhanlde�򿪵ľ��
* @pre \e a_pstHost ����Ϊ NULL
* @pre \e a_pstOrmPack ����ΪNULL
* @pre \e a_pstMeta ����ΪNULL
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*/
int torm_pack_record(IN TORMPROTOCOLHANDLE a_hOrmHandler, INOUT LPTORMSUBPACK a_pstOrmPack, IN LPTDRDATA a_pstRecord, IN LPTORMENTRYNAMELIST a_pstEntryList, IN LPTDRMETA a_pstMeta, IN int a_iVersion);


/** �ӱ��������г�ȡ������Ϣ���д��
*@param[in]	a_pstMeta �������ݵ�Ԫ�����������
*@param[in,out] a_pstPrimarykey ���������ݵĽṹ֧��
*	- ����  a_pstPrimarykey.pszBuff ���մ����Ϣ�Ļ������׵�ַ
*	- ����  a_pstPrimarykey.iBuff ���մ����Ϣ�Ļ�������󳤶�
*	- ���  a_pstPrimarykey.iBuff  ʵ��������Ϣ���ܳ���
*@param[in] a_pstData ����ṹ�����ݻ������ṹָ��
*	- ����	a_pstData.pszBuff ����ṹ�����ݵĻ�������ʼ��ַ
*	- ����  a_pstData.iBuff	ָ���������Ĵ�С
*@param[in]	a_iVersion	 �������ݵİ汾�������ֵΪ0������߰汾����
*
* @pre \e a_pstMeta ����ΪNULL
* @pre \e a_pstPrimarykey ����Ϊ NULL
* @pre \e a_pstData ����ΪNULL
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ��
*/
int torm_pack_primarykey(IN TORMPROTOCOLHANDLE a_hOrmHandler, IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstPrimarykey, IN LPTDRDATA a_pstData, int a_iVersion);


/** �Ӵ����������Ϣ�з����Ƿ�������������ݱ�������Ϣ
*@param[in] a_hOrmHandler	ORMЭ�鴦����
*@param[in]	a_pstMeta �������ݵ�Ԫ�����������
*@param[in] a_pstPrimarykey ���������ݵĽṹ֧��
*	- ����  a_pstPrimarykey.pszBuff ���մ����Ϣ�Ļ������׵�ַ
*	- ����  a_pstPrimarykey.iBuff ���մ����Ϣ�Ļ�������󳤶�
*
* @pre \e a_hOrmHandler ������ͨ��tdr_open_dbhanlde�򿪵ľ��
* @pre \e a_pstMeta ����ΪNULL
* @pre \e a_pstPrimarykey ����Ϊ NULL
*
*@return �������������������Ϣ������TDR_TRUE;���򷵻�TDR_FALSE
*/
TDRBOOLEAN torm_is_whole_primarykey(IN TORMPROTOCOLHANDLE a_hOrmHandler, IN LPTDRMETA a_pstMeta, IN LPTORMSLIGHTPACK a_pstPrimarykey);


/**�����ݿ�����blob��ŵ�struct������ڴ�struct�ṹ
*@param[in] a_pstMeta ���ݵ��������
*@param[in,out] a_pstSql �洢�����ݿ���ȡ�õĴ����������
*        - ����		a_pstSql.pszBuff	ָ�������Ϣ�Ļ�������ʼ��ַ
*        - ����		a_pstSql.iBuff		ָ���������Ĵ�С
*@param[in] a_pstData ����ת�������ݵĻ�����
*        - ����		a_pstData.pszBuff	����ת�����ݵĻ�������ʼ��ַ
*        - ����		a_pstData.iBuff 	ָ���������Ĵ�С
*        - ���		a_pstData.iBuff		ʵ�ʽ�������ݵ��ܳ���
*@param[in] a_iVersion   ���ð汾
*
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*@retval <0  ����ʧ��
*
*/
int torm_dbms_sql2struct(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstData, IN LPTDRDATA a_pstSql, IN int a_iVersion);


/**�����ݿ�����blob��ŵ�union������ڴ�����(ͨ������½��鲻Ҫ��union�������ݿ�)
*@param[in] a_pstMeta ���ݵ�Ԫ�����������
*@param[in] a_iSelectId union�ο���ID
*@param[in,out] a_pstSql �洢�����ݿ���ȡ�õĴ����������
*        - ����		a_pstSql.pszBuff 	ָ�������Ϣ�Ļ�������ʼ��ַ
*        - ����		a_pstSql.iBuff		ָ���������Ĵ�С
*@param[in] a_pstData ����ת�������ݵĻ�����
*        - ����		a_pstData.pszBuff	����ת�����ݵĻ�������ʼ��ַ
*        - ����		a_pstData.iBuff		ָ���������Ĵ�С
*        - ���		a_pstData.iBuff		ʵ�ʽ�������ݵ��ܳ���
*@param[in] a_iVersion   ���ð汾
*
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*@retval <0  ����ʧ��
*
*/
int torm_dbms_sql2union_ext(IN LPTDRMETA a_pstMeta, IN int a_iSelectId,  
						INOUT LPTDRDATA a_pstData, IN LPTDRDATA a_pstSql, IN int a_iVersion);


/**�����ݿ�����blob��ŵ�union������ڴ�����(ͨ������½��鲻Ҫ��union�������ݿ�)
*@param[in] a_pstMeta ���ݵ�Ԫ�����������
*@param[in] a_pszEntryName ������union��Աmeta����
*@param[in,out] a_pstSql �洢�����ݿ���ȡ�õĴ����������
*        - ����		a_pstSql.pszBuff	ָ�������Ϣ�Ļ�������ʼ��ַ
*        - ����		a_pstSql.iBuff		ָ���������Ĵ�С
*@param[in] a_pstData ����ת�������ݵĻ�����
*        - ����		a_pstData.pszBuff	����ת�����ݵĻ�������ʼ��ַ
*        - ����		a_pstData.iBuff		ָ���������Ĵ�С
*        - ���		a_pstData.iBuff		ʵ�ʽ�������ݵ��ܳ���
*@param[in] a_iVersion   ���ð汾
*
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*@retval <0  ����ʧ��
*
*/
int torm_dbms_sql2union(IN LPTDRMETA a_pstMeta, IN char* a_pszEntryName,  INOUT LPTDRDATA a_pstData, IN LPTDRDATA a_pstSql, IN int a_iVersion);


/** @}*/ /*defgroup TORM_DBAPI TORM_�����ϵӳ��*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
