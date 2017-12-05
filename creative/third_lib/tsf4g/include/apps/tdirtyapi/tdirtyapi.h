/**
 *
 * @file	tdirtyapi.h
 * @brief
 *
 * @author sean <sean@tencent.com>
 * @version 1.6
 * @date 2010-02-02
 *
 *
 * Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
 * All rights reserved.
 *
 */

#ifndef __TDIRTYAPI_H__
#define __TDIRTYAPI_H__

#if defined(_WIN32) || defined(_WIN64)
#ifdef _DEBUG
	#pragma comment(lib, "libtdirty_d.lib")
#else
	#pragma comment(lib, "libtdirty.lib")
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
  * tdirtyapi			��ʼ���ӿ�
  *
  * @param pszDirtyFile	�����ļ�·��, ����NULL��ʾ��tagent��ʼ��
  * @param uiBusid		�����������Ŀ��ҵ��id
  * @return 0			�ɹ�
			-1		�����ڴ�ʧ��
			-2		����tagent  ʧ��
			-3		�ļ�����ʧ��
  */

int tInitializeDirtyCheck(char * pszDirtyFile, unsigned int uiBusid);

/**
  * tDirtyCheck			GBK�ַ��������ۺϼ��ӿ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @return 0			���ɹ����໰
			1		�໰�������Ĺؼ���
			2		�໰����Ӣ�Ĺؼ���
			3		�໰������Ӣ��Ϲؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheck(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyReplace		GBK�ַ��������ۺ��滻�ӿ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @return 0 			���˳ɹ�, ����������ֽ�pszContent�������滻Ϊ*
			-1 		δ��ʼ������
			-2 		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyReplace(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyReplace2		GBK�ַ��������ۺ��滻�ӿڶ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel  �����໰����
  * @return 0 			���˳ɹ�, ����������ֽ�pszContent�������滻Ϊ*
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyReplace2(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * utf8DirtyCheck		UTF-8�ַ��������ۺϼ��ӿ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���UTF-8�ַ���
  * @param iDirtyLevel	�����໰����
  * @return 0			���ɹ�, ���໰
			1		�໰�������Ĺؼ���
			2		�໰����Ӣ�Ĺؼ���
			3		�໰������Ӣ��Ϲؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
		   -100	   ��ʼ����Ч����
           -200    �ַ���ת��ʧ��
  */

int utf8DirtyCheck	(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);

/**
  * utf8DirtyReplace	UTF-8�ַ��������ۺ��滻�ӿ�
  *
  * @param iDoorLevel	    �����໰����
  * @param pszUTF8Content	�����ַ���UTF-8�ַ���
  * @param iDirtyLevel      �����໰����
  * @param iBuffLen         pszUTF8Content�ַ������ڻ���������󳤶�,���ʾ����strlen(pszUTF8Content)����
  * @return 0 			���˳ɹ�, ����������ֽ�pszUTF8Content�������滻Ϊ*
  		 -1 			δ��ʼ������
  		 -2 			���ֿ���Ч����
         -4         ָ����iBuffLen��С����
         -100		��ʼ����Ч����
        -200        �ַ���ת��ʧ��
  */

int utf8DirtyReplace(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);
int utf8DirtyReplaceEx(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel, int iBuffLen);


/**
  * tDirtySetAutoUpdate	ʹ��tagent�Զ��������ֽӿ�
  *
  * @param iAutoUpdate	0 ���Զ�����1 �Զ�����
  * @return
  */

void tDirtySetAutoUpdate(int iAutoUpdate);

/**
  * tDirtySetCompCondition	�������ֱ�iDirtyLevel ��iDoorLevel �����ϵ�ӿ�
  *
  * @param iCond 0 iDirtyLevel == iDoorLevel , 1 iDirtyLevel >= iDoorLevel , -1 iDirtyLevel >= iDoorLevel, 1 Ĭ��
  * @return
  */

void tDirtySetCompCondition(int iCond);

/**
  * tDirtyUpdateCheck	�ֶ��������ֽӿ�, ���øýӿڳ��������ָ���
  *
  * @param
  * @return
  */

int tDirtyUpdateCheck();

/**
  * tGetDirtywordVersion	��ȡ���ֱ�汾��Ϣ
  *
  * @param
  * @return ��ǰ���ֱ�md5ֵ16�ֽڱ�ʾ
  */

const char *tGetDirtywordVersion();

/**
  * tDirtyCheckChn		GBK�ַ����������ּ��ӿ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel  �����໰����
  * @return 0			���ɹ�, ���໰
			1		�໰�������Ĺؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckChn(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckEng		GBK�ַ���Ӣ�����ּ��ӿ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @return 0			���ɹ�, ���໰
			1		�໰����Ӣ�Ĺؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckEng(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckReg		GBK�ַ���������ʽ���ӿ�win32��Ч
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel  �����໰����
  * @return 0			���ɹ�, ���໰
			1		�໰��������ؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckReg(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckPinyin	GBK�ַ���ƴ�����ӿ�win32��Ч
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @return 0			���ɹ�, ���໰
			1		�໰����ƴ���ؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/**
  * tDirtyCheckEx		GBK�ַ��������ۺϼ��ӿ�
  *
  * @param iDoorLevel	�����໰����, ��0���ֱ����������ֶ��ɹ���
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @param iBegIdx	����������ʼ�±�
  * @param iEndIdx	�����໰��ֹ�±�
  * @return 0			���ɹ�, ���໰
			1		�໰�������Ĺؼ���
			2		�໰����Ӣ�Ĺؼ���
			3		�໰������Ӣ��Ϲؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/**
  * tDirtyCheckChnEx	GBK�ַ����������ּ��ӿ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @param iBegIdx	����������ʼ�±�
  * @param iEndIdx	�����໰��ֹ�±�
  * @return 0			���ɹ�, ���໰
			1		�໰�������Ĺؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/**
  * tDirtyCheckEngEx	GBK�ַ���Ӣ�����ּ��ӿ�
  *
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @param iBegIdx	����������ʼ�±�
  * @param iEndIdx	�����໰��ֹ�±�
  * @return 0 			���ɹ�, ���໰
			1 		�໰����Ӣ�Ĺؼ���
			-1 		δ��ʼ������
			-2 		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/**
 * tMakeMapCode 	���ַ�ת��Ϊ��Ӧ������ӳ���������ӳ����
 * @param pszCharacter     Դ�ַ���gbk�ַ���
 * @param iMaxCodeLenth	   �����ӳ�����ַ�����������Ƴ���
 * @param pszSpellCode	   ת���������ӳ�����ַ���
 * @param pszPinyinCode    ת���������ӳ�����ַ���
 * @return 0	 ת���ɹ�
           1	 ����ת���ɹ������������̫С
          -1     �����������
 * @note 	���в���ָ�벻��ΪNULL��iMaxCodeLenth��Ϊ��ֵ
 */

int tMakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth);

/**
  * tDirtyFini tdirtyapi finish�ӿ�
  *
  * @param
  * @return
  */

int tDirtyFini();

/**
  * utf8_strlen UTF-8�ַ���ʵ���ȼ���
  *
  * @param s UTF-8�����ַ���
  * @return > 0 UTF-8���볤��
  */

size_t utf8_strlen(const char *s);

/**
  * utf8_strstr UTF-8�ַ������Һ���
  *
  * @param s1 UTF-8�����ַ���1
  * @param s2 UTF-8�����ַ���2
  * @param n s2������
  * @return s2����s1����ַ
  */

const char* utf8_strstr(const char *s1, const char *s2, size_t n);

/**
  * utf8SpecialCharFilterInit UTF-8�ַ�����������ʼ���ӿ�
  *
  * @param piHandle ����������handle
  * @param pszFileName �������ļ�
  * @return 0 �ɹ�-1 ʧ��
  */

int utf8SpecialCharFilterInit(int* piHandle, const char * pszFileName);

/**
  * utf8SpecialCharFilterScan UTF-8�ַ������������˽ӿ�
  *
  * @param iHandle ������handle
  * @param pszString �����ַ�
  * @return 0  ���˴������ں������ַ�1 �����ں������ַ�
  */

int utf8SpecialCharFilterScan(int iHandle, const char *pszString);

/**
  * utf8SpecialCharFilterFini UTF-8�ַ����������ͷŽӿ�
  *
  * @param iHandle ������handle
  * @return
  */

void utf8SpecialCharFilterFini(int* piHandle);

/**
  * tdirtyap i��ʼ���ӿ���Ҫ����win32�ͻ��˻���
  *
  * @param pszBuff	���ֱ��ڴ�ӳ��ָ�����\0��β
  * @param iBuff		���ֱ��ڴ�ӳ�񳤶�
  * @return 	0		�ɹ�
			-1		�����ڴ�ʧ��
  */

int tInitializeDirtyCheckEx(char * pszBuff,  int iBuff);

/**
  * tdirtyapi			�̰߳�ȫ��ʼ���ӿ�
  *
  * @param pszDirtyFile	�����ļ�·��, ����NULL��ʾ��tagent��ʼ��
  * @param uiBusid		�����������Ŀ��ҵ��id
  * @return >   0		�ɹ����ؾ��
			-1		�����ڴ�ʧ��
			-2		����tagent  ʧ��
			-3		�ļ�����ʧ��
  */
int tInitializeDirtyCheck_r	(char * pszFile, unsigned int uiBusid);

/**
  * tDirtyFini_r tdirtyapi  �̰߳�ȫfinish�ӿ�
  *
  * @param	iHandle	��ʼ�����ؾ��
  * @return
  */

int tDirtyFini_r				(int iHandle);

/**
  * tDirtyCheck		GBK�ַ��������ۺϼ���̰߳�ȫ�ӿ�
  *
  * @param iHandle	��ʼ���߳̾��
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @return 0			���ɹ����໰
			1		�໰�������Ĺؼ���
			2		�໰����Ӣ�Ĺؼ���
			3		�໰������Ӣ��Ϲؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheck_r			(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyReplace		GBK�ַ��������ۺ��滻�̰߳�ȫ�ӿ�
  *
  * @param iHandle	��ʼ���߳̾��
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel	�����໰����
  * @return 0 			���˳ɹ�, ����������ֽ�pszContent�������滻Ϊ*
			-1 		δ��ʼ������
			-2 		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyReplace_r			(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);


/**
  * tDirtyCheckReg_r	GBK�ַ���������ʽ�̰߳�ȫ���ӿ�win32��Ч
  *
  * @param iHandle	��ʼ���߳̾��
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel  �����໰����
  * @return 0			���ɹ�, ���໰
			1		�໰��������ؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckReg_r		(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckPiny_r	GBK�ַ���������ʽ���ӿ�win32��Ч
  *
  * @param iHandle	��ʼ���߳̾��
  * @param iDoorLevel	�����໰����
  * @param pszContent	�����ַ���GBK�ַ���
  * @param iDirtyLevel  �����໰����
  * @return 0			���ɹ�, ���໰
			1		�໰��������ؼ���
			-1		δ��ʼ������
			-2		���ֿ���Ч����
			-100	��ʼ����Ч����
  */

int tDirtyCheckPiny_r		(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyUpdateCheck_r	�ֶ��������ֽӿ�, ���øýӿڳ��������ָ���
  *
  * @param iHandle	��ʼ���߳̾��
  * @return
  */

int tDirtyUpdateCheck_r	(int iHandle);

#ifdef __cplusplus
}
#endif

#endif /* __TDIRTYAPI_H__ */

