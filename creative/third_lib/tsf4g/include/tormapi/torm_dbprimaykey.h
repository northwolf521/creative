/**
*
* @file     torm_dbprimaykey.h  
* @brief    �������ݿ����ϵͳ������Ϣ
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

#ifndef TORM_DBPRIMAYKEY_H
#define TORM_DBPRIMAYKEY_H

#include "tdr/tdr_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TORM_MAX_STRING_KEY_DATA	1024	/**< �ַ���������Ա������ݳ��� */
#define TORM_PRIMAYKEY_MAX_ENTRY	8		/**< ������ݿ�����������Ԫ����*/

/**DB�ؼ������Ͷ��� 
*/
enum tagTormDBKeyType
{
	TORM_DBKEY_TYPE_INT32	= 0,	/**< 32λ��������*/	
	TORM_DBKEY_TYPE_INT64,		/**< 64λ��������*/
	TORM_DBKEY_TYPE_FLOAT,		/**< ������*/
	TORM_DBKEY_TYPE_STR,		/**< �ַ�������,��NULL�ַ���β*/
	TORM_DBKEY_TYPE_BIN,		/**< ����������*/
};
typedef enum tagTormDBKeyType	ENRORMDBKEYTYPE;

union tagTormDBKeyData
{
	int iData;
	tdr_longlong llData;
	float fData;
	char szData[TORM_MAX_STRING_KEY_DATA];
	struct 
	{
		int iLen;
		char bData[TORM_MAX_STRING_KEY_DATA];
	}binData;
};
typedef union tagTormDBKeyData TORMDBKEYDATA;

struct  tagTormDBKeyInfo
{
	ENRORMDBKEYTYPE enType;
	TORMDBKEYDATA stData;
};
typedef struct  tagTormDBKeyInfo  TORMDBKEYINFO;
typedef struct  tagTormDBKeyInfo  *LPTORMDBKEYINFO;

struct tagTormPrimayKeyInfo
{
	int iNum;		/*��������ĳ�Ա��*/
	TORMDBKEYINFO stKeys[TORM_PRIMAYKEY_MAX_ENTRY];
};
typedef struct tagTormPrimayKeyInfo TORMPRIMAYKEYINFO;
typedef struct tagTormPrimayKeyInfo *LPTORMPRIMAYKEYINFO;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
