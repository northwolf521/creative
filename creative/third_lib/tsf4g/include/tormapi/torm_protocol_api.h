/**
*
* @file     torm_protocol_api.h  
* @brief    TORM������ǰ�˽����ʹ�õ�ͨ��Э�鴦��ӿ�ͷ�ļ�
*	�˽ӿ������Ϣ����ͽ��빤��
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

#ifndef TORM_PROTOCOL_API_H
#define TORM_PROTOCOL_API_H

#include <stdio.h>
#include "tlog/tlog.h"
#include "tormapi/torm_error.h"
#include "tormapi/torm_dbprimaykey.h"
#include "tormapi/torm_protocol.h"

#define TORM_PROTOCOL_HEAD_META	"TORMPkgHead"  /**<ORMЭ��ͷ��Ԫ��������������*/
#define TORM_PROTOCOL_BODY_META	"TORMPkgBody"	/**<ORMЭ����Ϣ��Ԫ��������������*/
#define TORM_PROTOCOL_PKG_META	"TORMPkg"	/**<ORMЭ����ϢԪ��������������*/


//#define TORM_PRO_DEFAULT_LOGCNF_FILE	"tormapilog.xml"  /*tormapiȱʡ����־ϵͳ�����ļ���*/
//#define TORM_PRO_LOGCAT_NAME	"tsf4g.tormapi"  /*tormapiȱʡ����־ϵͳ����־�������*/

#ifdef WIN32

# ifdef _DEBUG
#  define LIB_D    "_d"
# else
#  define LIB_D
# endif /* _DEBUG */

# if defined(LIB_D) 
# pragma comment( lib, "libtormapi"  LIB_D ".lib" )
# else
# pragma comment( lib, "libtormapi.lib" )
# endif

#endif



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**ORMЭ����Ϣ������ 
*/
typedef unsigned int TORMPROTOCOLHANDLE;
typedef TORMPROTOCOLHANDLE *LPTORMPROTOCOLHANDLE;


/**@defgroup  TORM_CS_METALIB_MANAGE TORM_Э�鴦������Ԫ��������
* @{
*/

/** ���ݴ���ڹ����ڴ��е�Ԫ���������ⴴ��Э�鴦����
*@note ��֧��windows��ʹ�ã�windows����ʹ��torm_create_handle_files
*@param[in,out] a_phTormHandler ��ȡ����������Э�鴦������ָ��
*@param[in] a_pszKey	����Ԫ���ݿ�Ĺ����ڴ��keyֵ��
*@param[in] a_paszLibNames �����洢���ݵ�Ԫ���ݿ������б�
*@param[in] a_iLibNum �����洢���ݵ�Ԫ���ݿ�ĸ���
*@param[in] a_fpError �������ʧ�ܣ�ͨ���˾��ָ�����ļ�����¼������Ϣ
*
*@note ���ӿڴ�����Э�鴦����ֻ����ͬһ���߳���ʹ�ã�������̲߳��ܹ���ͬһ��Э�鴦����
*@note ��Ϣ�����ӿ�ʹ������Ԫ���ݿ�:
*	- ����һ��Ԫ���ݿ�����ORM��������ͻ���֮���ͨ��Э�飬��Ԫ���ݿ��뱾�ӿ��Ѿ�������һ��,�������
*	- ��һ��Ԫ���ݿ������洢���ݿ�ϵͳ�е����ݣ����ӿ�ʹ��tsf4g/tmngģ����������ЩԪ���ݿ⡣���ñ���
*	ORMЭ��ӿڵ�Ӧ�ó�������ǰ����ʹ��tsf4g/tmng/agent�ṩ�Ĺ��߼��ص������ڴ棬��������Ԫ���ݿ�����ͬʱ���ڶ����
*
*@note Э�鴦�����ɹ�������ʹ����ϱ������torm_delete_handle�ͷž������������ڴ���Դй¶��
*@note �����Ҫ��¼API�е���־�����ڵ��ñ��ӿ�֮�����torm_set_handler_logcat�ӿ�
*
*@return 0 �ɹ�; ���㣬ʧ��
*
*@see torm_delete_handle
*@see torm_open_metalib
*@see tsf4g/tmng
*
*@pre \e a_phTormHandler����ΪNULL
*@pre \e a_pszKey ����ΪNULL����ָ�Ĺ����ڴ��Ѿ������ã����Ҵ洢���ݵ�Ԫ�����������Ѿ����ص��˹����ڴ���
*@pre \e a_paszLibNames ����ΪNULL����ǰa_iLibNum��Ԫ���ݿ�����ΪNULL
*@pre \e a_iLibNum �������0
*@pre \e ������ڶ�������洢���ݵ�Ԫ���������⣬����ЩԪ���ݿ��г��ֵ�Ԫ���ݽṹ���Ʋ����ظ�
*
*@par ʾ������:
*  @code
	�ϲ�Ӧ��ʹ�ñ��׽ӿڵĴ����������£�
	//step1:��ʼ��Э�鴦����
	TORMPROTOCOLHANDLE hTormHandler;
	char *aszDataLib[] = {"ZoneDBData","WordDBData"};

	torm_create_handle(&hTormHandler, "666666", aszDataLib, 2,"torm_net");

	//step2: ������־���
	torm_set_handler_logcat(hTormHandler, ...);

	//����3-7�ظ�ִ��

	//step3: ��������
	TORMPKG stPkg;
	......
	stPkg.Head.HeadCmd.Cmd = TORM_SELECT_REQ;
	......
	......
	torm_make_req_msg(hTormHandler, &stNetMsgInfo,......);

	//step4: ͨ��tbus��������,��Ϣ��󳤶�Ϊsizeof(TORMPKG)
	send_orm_msg(stNetMsgInfo);
	........

	//step5: ͨ��tbus��ȡ��Ϣ����,��Ϣ��󳤶�Ϊsizeof(TORMPKG)
	recv_orm_msg(stNetMsgInfo);
	....

	//step6: ��������Ϣ�󶨵����
	torm_attach_msg(hTormHandler, &stNetMsgInfo, &stPkg,0);

	//step7: ���ݲ���
	for(;;)
	{
		...........
	}
	
	//step8��ʹ����ϣ��ͷ�Э�鴦����
	torm_delete_handle(&hTormHandler);

*  @endcode
*@see torm_set_logcat
*/
int torm_create_handle(INOUT LPTORMPROTOCOLHANDLE a_phTormHandler, IN const char *a_pszKey, 
					   IN char *a_paszLibNames[], IN int a_iLibNum, IN FILE *a_fpError);


/** ���ݶ����Ƹ�ʽԪ���������ⴴ��Э�鴦����
*@param[in,out] a_phTormHandler ��ȡ����������Э�鴦������ָ��
*@param[in] a_paszLibFile �����洢���ݵĶ�����Ԫ���ݿ��ļ����б�
*@param[in] a_iFileNum �����洢���ݵ�Ԫ���ݿ�ĸ���
*@param[in] a_fpError �������ʧ�ܣ�ͨ���˾��ָ�����ļ�����¼������Ϣ
*
*@note ���ӿڴ�����Э�鴦����ֻ����ͬһ���߳���ʹ�ã�������̲߳��ܹ���ͬһ��Э�鴦����
*@note ��Ϣ�����ӿ�ʹ������Ԫ���ݿ�:
*	- ����һ��Ԫ���ݿ�����ORM��������ͻ���֮���ͨ��Э�飬��Ԫ���ݿ��뱾�ӿ��Ѿ�������һ��,�������
*	- ��һ��Ԫ���ݿ������洢���ݿ�ϵͳ�е����ݣ���Ҫ��ǰͨ��tdr�������ɺ��ɱ��ӿڼ���
*
*@note Э�鴦�����ɹ�������ʹ����ϱ������torm_delete_handle�ͷž������������ڴ���Դй¶��
*
*@return 0 �ɹ�; ���㣬ʧ��
*
*@see torm_create_handle
*@see torm_delete_handle
*
*@pre \e ����ԭ���ݿ�����Ʋ�����
*@pre \e a_phTormHandler����ΪNULL
*@pre \e a_paszLibFile ����ΪNULL�������Ƶ�Ԫ����������ʹ��TDR�Ĺ���tdr����
*@pre \e a_iFileNum �������0
*@pre \e a_pszOrmProLib ����ΪNULL,���Ҵ洢���ݵ�Ԫ�����������Ѿ����ص��˹����ڴ���
*@pre \e ������ڶ�������洢���ݵ�Ԫ���������⣬����ЩԪ���ݿ��г��ֵ�Ԫ���ݽṹ���Ʋ����ظ�
*
*/
int torm_create_handle_files(INOUT LPTORMPROTOCOLHANDLE a_phTormHandler, IN char *a_paszLibFile[], IN int a_iFileNum, IN FILE *a_fpError);

/**���Э�鴦������Ԫ���ݴ�С�������Ԫ�������л�֮������С����MaxSize���ʧ��
*@param[in] a_phTormHandler ����Э�鴦������ָ��
*
*@return 0 �ɹ�; ���㣬ʧ��
*/
int torm_check_handle_metasize_limit(IN TORMPROTOCOLHANDLE a_hTormHandler, IN int iMaxSize);


/**����Э�鴦���������ͷ���Դ
*@param[in] a_phTormHandler ����Э�鴦������ָ��
*
*/
void torm_destroy_handle(INOUT LPTORMPROTOCOLHANDLE a_phTormHandler);


/**��Э�鴦���������ݵ������ļ����Ա����
*@param[out] a_fpFile ���浼����Ϣ���ļ����
*@param[in] a_hTormHandler ����Э�鴦����
*
*/
void torm_dump_handle(OUT FILE *a_fpFile,  IN TORMPROTOCOLHANDLE a_hTormHandler);


/**����Ԫ��������ȡ�������ݵ�Ԫ�����������
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_pszMetaName �������ݵ�Ԫ��������
*
*@pre \e a_phTormHandler����ΪNULL
*@pre \e a_pszLibName ����ΪNULL
*
*@return �������ָ�������ֵ�Ԫ���ݣ��򷵻����������򷵻�NULL
*/
LPTDRMETA torm_get_meta(IN TORMPROTOCOLHANDLE a_hTormHandler, IN const char *a_pszMetaName);


/**��Э��������־���
@param[in] a_hTormHandler -- Э�鴦���� 
@param[in] a_pstLogCat -- ��־���
*/
void torm_set_handler_logcat( IN TORMPROTOCOLHANDLE a_hTormHandler, IN LPTLOGCATEGORYINST a_pstLogCat);


/**@} */ /*  defgroup  TORM_CS_METALIB_MANAGE TORM_Ԫ������Դ����*/


/**@defgroup  TORM_CS_MSG_PACK_UNPACK_PUB TORM_Э����Ϣ����_cs�����ӿ�
* @{
*/
/** 
*/

/**��������Ϣ�н�����ORMЭ����Ϣ��ͷ��
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_pstPkgHead ORMЭ����Ϣͷ���ṹ��ָ��
*@param[in] a_pszNetMsg ����������Ϣ�Ļ�����ָ��
*@param[in,out] a_piMsgLen ������Ϣ������������Ϣ 
*	- ���� ������������Ϣ����
*	- ��� �Խ�����������Ϣ����
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_pstPkgHead����ΪNULL
*@pre \e a_pszNetMsg ����ΪNULL
*@pre \e a_piMsgLen ����ΪNULL
*@pre \e *a_piMsgLen�������0
*@pre \e a_hTormHandler ����ΪNULL
*
*@return 0���ɹ������㣺ʧ��
*/
int torm_fetch_pkghead(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTORMPKGHEAD a_pstPkgHead, IN const char *a_pszNetMsg, INOUT int *a_piMsgLen, IN int a_iVersion);

/**�Կ��ӻ��Ĵ�ӡЭ������ͷ������ӡ�����ָ���ļ���
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_fpFile �����ӡ���ݵ��ļ����
*@param[in] a_pstPkgHead ��Ϣͷ���ṹ��ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_fpFile����ΪNULL
*@pre \e a_pstPkg ����ΪNULL
*@pre \e a_hTormHandler ����ΪNULL
*
*@return 0���ɹ������㣺ʧ��
*/
int torm_fprintf_pkghead(IN TORMPROTOCOLHANDLE a_hTormHandler, IN FILE* a_fpFile, IN LPTORMPKGHEAD a_pstPkgHead, IN int a_iVersion);

/**�Կ��ӻ��Ĵ�ӡЭ������ͷ������ӡ�����ָ����������
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_pszBuf �����ӡ���ݵĻ��������
*@param[in,out] a_piBuf �����ӡ���ݵĻ�����������Ϣ
*	- ���� ����������ʹ����󳤶�
*	- ��� ��������ʹ�ó���
*@param[in] a_pstPkgHead ��Ϣͷ���ṹ��ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_pszBuf����ΪNULL
*@pre \e a_pstPkg ����ΪNULL
*@pre \e a_pstPkgHead ����ΪNULL
*@pre \e a_hTormHandler ����ΪNULL
*@pre \e *a_piBuf�������0 
*
*@return 0���ɹ������㣺ʧ��
*/
int torm_sprintf_pkghead(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszBuf, INOUT int* a_piBuf, IN LPTORMPKGHEAD a_pstPkgHead, IN int a_iVersion);


/**�Կ��ӻ��Ĵ�ӡЭ�����ݣ���ӡ�����ָ���ļ���
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_fpFile �����ӡ���ݵ��ļ����
*@param[in] a_pstPkg ��Ϣ�ṹ��ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_fpFile����ΪNULL
*@pre \e a_pstPkg ����ΪNULL
*@pre \e a_hTormHandler ����ΪNULL
*
*@return 0���ɹ������㣺ʧ��
*/
int torm_fprintf_pkg(IN TORMPROTOCOLHANDLE a_hTormHandler, IN FILE* a_fpFile, IN LPTORMPKG a_pstPkg, IN int a_iVersion);

/**�Կ��ӻ��Ĵ�ӡЭ�����ݣ���ӡ�����ָ����������
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_pszBuf �����ӡ���ݵĻ��������
*@param[in,out] a_piBuf �����ӡ���ݵĻ�����������Ϣ
*	- ���� ����������ʹ����󳤶�
*	- ��� ��������ʹ�ó���
*@param[in] a_pstPkg ��Ϣ�ṹ��ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_pszBuf����ΪNULL
*@pre \e a_pstPkg ����ΪNULL
*@pre \e a_hTormHandler ����ΪNULL
*@pre  \e a_piBuf ����ΪNULL
*@pre \e *a_piBuf�������0 
*
*@return 0���ɹ������㣺ʧ��
*/
int torm_sprintf_pkg(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszBuf, INOUT int* a_piBuf, IN LPTORMPKG a_pstPkg, IN int a_iVersion);

/**��Э��������XML��ʽ�����ָ����������
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_pszXml �����ӡ���ݵĻ�������Ϣ
*@param[in,out] a_piXml �����ӡ���ݵĻ�����������Ϣ
*	- ���� ����������ʹ����󳤶�
*	- ��� ��������ʹ�ó���
*@param[in] a_pstPkg ��Ϣ�ṹ��ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_pszXml����ΪNULL
*@pre \e a_pstPkg ����ΪNULL
*@pre \e a_hTormHandler ����ΪNULL
*@pre \e a_piXml ����ΪNULL
*@pre \e *a_piXml �������0
*
*@return 0���ɹ������㣺ʧ��
*/
int torm_output_pkg(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszXml, INOUT int* a_piXml, IN LPTORMPKG a_pstPkg, IN int a_iVersion);
/**@} */ /*  defgroup  TORM_CS_MSG_PACK_UNPACK TORM_Э����Ϣ�����_����*/


/**@defgroup  TORM_CS_MSG_PACK_UNPACK_CLI TORM_Э����Ϣ����_�ͻ���ʹ��
* @{
*/

/**����Э��ṹ�����ݺͲ�����������ORM����������Ϣ
*@param[in] a_hTormHandler Э�鴦����
*@param[in,out]	a_pstNet �洢������Ϣ�ṹ��ָ��
*	- ����	a_pstNet.pszBuff ָ������������Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstNet.iBuff	ָ���������Ĵ�С
*	- ���  a_pstNet.iBuff  ʵ�ʱ���������Ϣ���ܳ���
*@param[in] a_pstPkg ��Ϣ�ṹ��ָ��
*@param[in]	a_pstMetaData �洢�������ݵĻ�������Ϣ,����CmdΪTORM_EXGVER_REQ��TORM_SVRINFO_REQ�����󣬱���������ΪNULL
*	- ����	a_pstMetaData.pszBuff ָ������������ݵĻ�������ʼ��ַ
*	- ����  a_pstMetaData.iBuff	ָ���������Ĵ�С
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@note ���ӿ���Ҫ��Ϊ�������ݲ��������룬���¡����ҡ�ɾ������������,����CmdΪTORM_EXGVER_REQ��TORM_SVRINFO_REQ��������Ե���
*	tdr_hton����������Ϣ�����ӿڵ���ҪĿ�����Զ�Ϊ�ϲ�����TORMOPERINFO�ṹ�е�PriKeyInfo��OperContent���ݣ��ϲ�ֻ��ָ��TORMOPERINFO�ṹ��
*	��EntryNameList��WhereDef��ResFlag�ֶμ��ɡ�
*
*@note ���ݲ��������룬���¡����ҡ�ɾ��������TORMOPERINFO�ṹEntryNameList�ֶ����������ϲ����Ȥ�����ݳ�Ա��Ԫ�������������б�������б�Ϊ�գ����ʾ
*	�Բ������ݵ����г�Ա����Ȥ��
*
*@note TORMOPERINFO�ṹWhereDef�ֶε����⣺
	- ���ݲ���������ORM�˻�������Ӧ��SQL��䣬
	- ���룬���¡����ҡ�ɾ������SQL�����������ܽ�Ϊ�ֳ��Ĳ��ֵ�ģʽ��Operate(Select/update/insert/delete) data_description table_description restriction_description(WHERE,ORDER,LIMIT)
	- WhereDef�ֶα���restriction_description��һ���ֵ���Ϣ���������ɶ��ϲ�Ӧ�����ξ���������SQLת��ϸ�ڣ�ͬʱ��Ϊ�ϲ�Ӧ�����ָ������Լ���ṩ�˿���
*
*@pre \e �������ݲ��������룬���¡����ҡ�ɾ�����������TORMOPERINFO�ṹWhereDef�ֶβ�Ϊ�գ������������ݲ��������Where����Լ����Ϣ
*@pre \e �Զ������ݲ��������룬���¡����ҡ�ɾ��������TORMOPERINFO�ṹWhereDef�ֶΣ���Ҫ�˽����������ݿ��еı�ṹ���壬��ʹ�ñ�ṹ�����е�����������Լ����F
*
*@return 0���ɹ������㣺ʧ��
*
*@par ʾ������:
*  @code
	�ٶ�ĳ���洢���ݵ�ԭ���������£�
	<struct name="UserBaseInfo" version="10" primarykey="Uin,RoleName" >
		<entry name="Uin" type="uint" />
		<entry name="RoleName" type="longlong" />
		<entry name="Level" type="int">
		<entry name="LastLogin" type="datetime" />
	</struct>

	��ORM�ͻ��˸���Uin��RoleName����ͬ��ɫ��Ϣ��α�������£�
	TORMPKG stPkg;
	USERBASEINFO stBaseInfo;
	char szNet[32765];
	TDRDATA stNetInfo;
	TDRDATA stMetaData;
	int iRet;

	stPkg.Head.HeadComm.Magic = TORM_MAGIC;
	stPkg.Head.HeadComm.InstID = 100;
	stPkg.Head.HeadCmd.Cmd = TORM_SELECT_REQ;
	strcpy(stPkg.Head.HeadCmd.MetaNameVer.MetaName,"UserBaseInfo");
	stPkg.Head.HeadCmd.MetaNameVer.MetaVer = 10;
	stPkg.Head.HeadApp.Seq = 100001
	stPkg.Head.HeadApp.ObjID = 2;

	stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryNameNum = 0;

	stBaseInfo.Uin = 10000;
	strcpy(stBaseInfo.RoleName, "test")

	stNetInfo.pszBuff = &szNet[0];
	stNetInfo.iBuff = sizeof(szNet);
	stMetaData.pszBuff =  &stBaseInfo;
	stBaseInfo.iBuff = sizeof(stBaseInfo);
	iRet = torm_make_req_msg(hTormHandler, &stNetInfo, &stPkg, &stBaseInfo,0);

	ָ�������Ĳ���α�������£�
	stPkg.Head.HeadComm.Magic = TORM_MAGIC;
	stPkg.Head.HeadComm.InstID = 100;
	stPkg.Head.HeadCmd.Cmd = TORM_SELECT_REQ;
	strcpy(stPkg.Head.HeadCmd.MetaNameVer.MetaName,"UserBaseInfo");
	stPkg.Head.HeadCmd.MetaNameVer.MetaVer = 10;
	stPkg.Head.HeadApp.Seq = 100001;
	stPkg.Head.HeadApp.ObjID = 2;

	stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryNameNum = 2;
	strcpy(stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryName[0],"Uin");
	strcpy(stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryName[0],"RoleName");

	strcpy(stPkg.Body.SelectReq.OperInfo.WhereDef,"Where Level>110 limit=100 Order by asc");

	stBaseInfo.Uin = 10000;

	stNetInfo.pszBuff = &szNet[0];
	stNetInfo.iBuff = sizeof(szNet);
	stMetaData.pszBuff =  &stBaseInfo;
	stBaseInfo.iBuff = sizeof(stBaseInfo);
	iRet = torm_make_req_msg(hTormHandler, &stNetInfo, &stPkg, &stBaseInfo,0);


*  @endcode
*@pre \e a_hTormHandler ����ΪNULL
*@pre \e a_pstNet ����ΪNULL
*@pre \e a_pstNet.pszBuff ����ΪNULL
*@pre \e a_pstNet.iBuff �������0
*@pre \e a_pstPkg ����ΪNULL
*/
int torm_make_req_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, IN LPTORMPKG a_pstPkg, IN LPTDRDATA a_pstMetaData, IN int a_iVersion);

/** ���ܺ�torm_make_req_msg������ͬ��ֻ���ڹ���������ʱ���torm_make_req_msg�෴��
  ���:
      stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryNameNum = 1;
      strcpy(stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryName[0],"LastLogin");
��: ��ʾ�������ǳ���LastLogin �ֶ�����������ֶ�
*/
int torm_make_req_msg_r(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, IN LPTORMPKG a_pstPkg, IN LPTDRDATA a_pstMetaData, IN int a_iVersion);




/**����ORM��Ӧ��Ϣ
* �� ����Cmd����TORM_SELECT_RES����Ϣ�����ӿ�ֱ�ӽ�������Ϣ������a_pstPkg�ṹ��
* -  ����CmdΪTORM_SELECT_RES����Ϣ�����ӿڽ���������Ϣ�н����������ҽ�����������Ա��ͨ������torm_fetch_one_meta�ӿڿ��ԴӼ��ص���Ӧ��Ϣ�з�������������
*@param[in] a_hTormHandler Э�鴦����
*@param[in,out]	a_pstNet �洢������Ϣ�ṹ��ָ��
*	- ����	a_pstNet.pszBuff ָ������������Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstNet.iBuff	ָ���������Ĵ�С
*	- ���  a_pstNet.iBuff  ʵ�ʱ���������Ϣ���ܳ���
*@param[in,out] a_pstPkg �����������Ϣ��Ϣ�Ľṹ���ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_hTormHandler����ΪNULL
*@pre \e a_pstNet ����ΪNULL
*@pre \e a_pstNet.pszBuff ����ΪNULL
*@pre \e a_pstNet.iBuff �������0
*@pre \e a_pstPkg ����ΪNULL
*
*@return 0���ɹ������㣺ʧ��
*
*@see torm_fetch_one_meta
*/
int torm_attach_res_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, INOUT LPTORMPKG a_pstPkg, IN int a_iVersion);

/**�Ӽ��ص�ORM ������Ӧ��ȡ��һ���ṹ������
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_pszBuf ����������ݵĻ�����
*@param[in] a_iBuf �����ӡ���ݵĻ������׵�ַ
*	- ���� ����������ʹ����󳤶�
*
*@note ���ô˽ӿڶ�ȡһ���ض�����,���ܵ����Ƿ�ɹ�,�Ժ�����ͨ���˽ӿڶ�ȡ�����ض�����
*@pre \e a_hTormHandler����ΪNULL
*@pre \e a_pszBuf ����ΪNULL
*@pre \e a_iBuf ����ָ��������С����������洢һ��Ԫ���ݽṹ��
*@pre \e ֮ǰ�������torm_attach_res_msg����ORM��Ӧ��Ϣ�������һ�ε���torm_attach_res_msg,��ǰһ�μ�����Ϣ�е����ݽ�ʧЧ,����ϲ�Ӧ�ñ���
* ȷ������һ�ε���torm_attach_res_msg֮ǰȡ��������������
*
**@return 0���ɹ������㣺ʧ��
*
*@return 0���ɹ������㣺ʧ��
*
*@see torm_attach_res_msg
*@par ʾ������:
*  @code
	��Select��ӦΪ����˵����Ϣ��������ݻ�ȡ��α���룺
	char szMsgBuff[246000];
	int iMsgLen;
	TDRDATA stNetInfo

	//����������Ϣ
	recv_orm_msg(fio, &szMsgBuff[0], &iMsgLen);
	........

	//�ַ���Ϣ
	TORMPKG stPkg;
	int iRet;

	stNetInfo.pszBuff = &szMsgBuff[0];
	stNetInfo.iBuff = iMsgLen;
	
	iRet = torm_attach_res_msg(hTormHandler, &stNetInfo, &stPkg, 0);
	switch(stPkg.Head.HeadCmd.Cmd)
	{
	case TORM_SELECT_RES:
		iRet = process_select_res(&stPkg);
		break;
		......
	default:
		break;
	}

	//��Ӧ��Ϣ����
	int process_select_res(LPTORMPKG pstPkg)
	{
		LPTDRMETA pstMeta;
		char *pszMetaBuff;
		int iSize;
		......

		//���ݲ�������������������ȡ�������ݵ��ڴ�ռ�
		pstMeta = torm_get_meta(hTormHandler, pstPkg->Head.HeadCmd.MetaNameVer.MetaName)
		if (m_pszMetaBuff == NULL)
		{
			m_pszMetaBuff �� get_buff(pstPkg->Body.SelectRes.SelectResult.ResultTotal* tdr_get_meta_size(pstMeta));
		}

		pszMetaBuff = get_free_meta_buff(m_pszMetaBuff);
		for(i = 0; i < pstPkg->Body.SelectRes.SelectResult.ResultNum;i++)
		{
			iSize = tdr_get_meta_size(pstMeta);
			torm_fetch_one_meta(hTormHandler, pszMetaBuff, &iSize);
			pszMetaBuff += tdr_get_meta_size(pstMeta);
		}
	}
*  @endcode
*/
int torm_fetch_one_meta(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszBuf, IN int a_iBuf);

/** �ӱ�torm_attach_res_msg��װ�Ĳ��������н�������ݵ��ڴ�ṹ��
*@param[in] a_hTormHandler Э�鴦����
*@param[in] a_pstMeta �����ڴ�ṹ��Ԫ���ݽṹ���
*@param[in] a_pstOperInfo ��װ�Ĳ������ݽṹ��ָ��
*@param[in,out]	a_pstHost �洢���ݵ��ڴ滺�����ṹ��ָ��
*	- ����	a_pstHost.pszBuff �洢���ݵ��ڴ滺������ʼ��ַ
*	- ����  a_pstHost.iBuff	��������ʹ�ô�С
*	- ���  a_pstHost.iBuff  ʵ��ʹ�õ��ڴ滺������С
*@param[in] a_iVersion ���ݽ�����ð汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_hTormHandler����ΪNULL
*@pre \e a_pstHost ����ΪNULL
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_pstOperInfo ����ΪNULL
*
*@return 0���ɹ������㣺ʧ��
*
*@see torm_attach_res_msg
*/
int torm_unpack_operinfo(IN TORMPROTOCOLHANDLE a_hTormHandler, IN LPTDRMETA a_pstMeta, IN LPTORMOPERINFO a_pstOperInfo, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);

/**@} */ /*  defgroup  TORM_CS_MSG_PACK_UNPACK TORM_Э����Ϣ�����_�ͻ���ʹ��*/


/**@defgroup  TORM_CS_MSG_PACK_UNPACK_SVR TORM_Э����Ϣ����_������ʹ��
* @{
*/
/**��������Ϣ�з�����������Ϣ 
*@param[in] a_hTormHandler Э�鴦����
*@param[in]	a_pstNet �洢������Ϣ�ṹ��ָ��
*	- ����	a_pstNet.pszBuff ָ������������Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstNet.iBuff	ָ���������Ĵ�С
*@param[in] a_pstPkgHead ��������Ϣ��ͷ���ṹ��ָ��.
*	-	����ϲ�Ӧ���Ѿ�����Ϣ��������Ϣͷ����ͨ���˲������ӿڽ��ӿ촦����̣�
*	-	�˲���ΪNULL���򱾽ӿڽ�����Ϣ�з�������Ϣͷ��
*@param[out] a_pstKeyInfo ����������Ϣ�Ľṹ��ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_hTormHandler����ΪNULL
*@pre \e a_pstNet ����ΪNULL
*@pre \e a_pstNet.pszBuff ����ΪNULL
*@pre \e a_pstNet.iBuff �������0
*@pre \e a_pstPkgHead ����ΪNULL
*@pre \e a_pstKeyInfo ����ΪNULL
*/
int  torm_get_primaykey(IN TORMPROTOCOLHANDLE a_hTormHandler, IN LPTDRDATA a_pstNet, IN LPTORMPKGHEAD a_pstPkgHead, OUT LPTORMPRIMAYKEYINFO a_pstKeyInfo, IN int a_iVersion);

/**ORMЭ����Ϣ����
*@param[in] a_hTormHandler Э�鴦����
*@param[in,out]	a_pstNet �洢������Ϣ�ṹ��ָ��
*	- ����	a_pstNet.pszBuff ָ������������Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstNet.iBuff	ָ���������Ĵ�С
*	- ���  a_pstNet.iBuff  ʵ�ʱ���������Ϣ���ܳ���
*@param[in] a_pstPkg ��Ϣ�ṹ��ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@return 0���ɹ������㣺ʧ��
*
*/
int torm_pack_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, IN LPTORMPKG a_pstPkg, IN int a_iVersion);

/**ORMЭ����Ϣ����
@param[in] a_hTormHandler Э�鴦����
*@param[in,out]	a_pstNet �洢������Ϣ�ṹ��ָ��
*	- ����	a_pstNet.pszBuff ָ������������Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstNet.iBuff	ָ���������Ĵ�С
*	- ���  a_pstNet.iBuff  ʵ�ʱ���������Ϣ���ܳ���
*@param[in,out] a_pstPkg �����������Ϣ��Ϣ�Ľṹ���ָ��
*@param[in] a_iVersion Э��汾�ţ������ֵΪ0���򰴵�ǰ��߰汾���н���
*
*@pre \e a_hTormHandler����ΪNULL
*@pre \e a_pstNet ����ΪNULL
*@pre \e a_pstPkg ����ΪNULL
*
*@return 0���ɹ������㣺ʧ��
*
*@see torm_fetch_one_meta
*/
int torm_unpack_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, INOUT LPTORMPKG a_pstPkg, IN int a_iVersion);

void torm_init_operate_info(INOUT LPTORMOPERINFO a_pstOpeInfo);

void torm_copy_operate_info(INOUT LPTORMOPERINFO a_pstOpeInfoOut, IN LPTORMOPERINFO a_pstOpeInfoIn);


/**@} */ /*  TORM_CS_MSG_PACK_UNPACK_SVR TORM_Э����Ϣ����_������ʹ��*/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
