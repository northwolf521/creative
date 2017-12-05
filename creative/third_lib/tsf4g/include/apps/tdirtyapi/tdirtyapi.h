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
 * Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
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
  * tdirtyapi			初始化接口
  *
  * @param pszDirtyFile	脏字文件路径, 传入NULL表示从tagent初始化
  * @param uiBusid		分配给各个项目的业务id
  * @return 0			成功
			-1		分配内存失败
			-2		调用tagent  失败
			-3		文件复制失败
  */

int tInitializeDirtyCheck(char * pszDirtyFile, unsigned int uiBusid);

/**
  * tDirtyCheck			GBK字符集脏字综合检查接口
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @return 0			检查成功非脏话
			1		脏话符合中文关键字
			2		脏话符合英文关键字
			3		脏话符合中英混合关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheck(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyReplace		GBK字符集脏字综合替换接口
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @return 0 			过滤成功, 如果存在脏字将pszContent中脏字替换为*
			-1 		未初始化错误
			-2 		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyReplace(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyReplace2		GBK字符集脏字综合替换接口二
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel  返回脏话级别
  * @return 0 			过滤成功, 如果存在脏字将pszContent中脏字替换为*
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyReplace2(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * utf8DirtyCheck		UTF-8字符集脏字综合检查接口
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串UTF-8字符集
  * @param iDirtyLevel	返回脏话级别
  * @return 0			检查成功, 非脏话
			1		脏话符合中文关键字
			2		脏话符合英文关键字
			3		脏话符合中英混合关键字
			-1		未初始化错误
			-2		脏字库无效错误
		   -100	   初始化无效错误
           -200    字符集转换失败
  */

int utf8DirtyCheck	(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);

/**
  * utf8DirtyReplace	UTF-8字符集脏字综合替换接口
  *
  * @param iDoorLevel	    门限脏话级别
  * @param pszUTF8Content	输入字符串UTF-8字符集
  * @param iDirtyLevel      返回脏话级别
  * @param iBuffLen         pszUTF8Content字符串所在缓冲区的最大长度,零表示采用strlen(pszUTF8Content)计算
  * @return 0 			过滤成功, 如果存在脏字将pszUTF8Content中脏字替换为*
  		 -1 			未初始化错误
  		 -2 			脏字库无效错误
         -4         指定的iBuffLen大小错误
         -100		初始化无效错误
        -200        字符集转换失败
  */

int utf8DirtyReplace(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);
int utf8DirtyReplaceEx(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel, int iBuffLen);


/**
  * tDirtySetAutoUpdate	使用tagent自动更新脏字接口
  *
  * @param iAutoUpdate	0 非自动更新1 自动更新
  * @return
  */

void tDirtySetAutoUpdate(int iAutoUpdate);

/**
  * tDirtySetCompCondition	设置脏字表iDirtyLevel 与iDoorLevel 级别关系接口
  *
  * @param iCond 0 iDirtyLevel == iDoorLevel , 1 iDirtyLevel >= iDoorLevel , -1 iDirtyLevel >= iDoorLevel, 1 默认
  * @return
  */

void tDirtySetCompCondition(int iCond);

/**
  * tDirtyUpdateCheck	手动更新脏字接口, 调用该接口程序检查脏字更新
  *
  * @param
  * @return
  */

int tDirtyUpdateCheck();

/**
  * tGetDirtywordVersion	获取脏字表版本信息
  *
  * @param
  * @return 当前脏字表md5值16字节表示
  */

const char *tGetDirtywordVersion();

/**
  * tDirtyCheckChn		GBK字符集中文脏字检查接口
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel  返回脏话级别
  * @return 0			检查成功, 非脏话
			1		脏话符合中文关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckChn(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckEng		GBK字符集英文脏字检查接口
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @return 0			检查成功, 非脏话
			1		脏话符合英文关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckEng(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckReg		GBK字符集正则表达式检查接口win32无效
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel  返回脏话级别
  * @return 0			检查成功, 非脏话
			1		脏话符合正则关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckReg(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckPinyin	GBK字符集拼音检查接口win32无效
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @return 0			检查成功, 非脏话
			1		脏话符合拼音关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/**
  * tDirtyCheckEx		GBK字符集脏字综合检查接口
  *
  * @param iDoorLevel	门限脏话级别, 填0脏字表中所有脏字都可过滤
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @param iBegIdx	返回脏字起始下标
  * @param iEndIdx	返回脏话终止下标
  * @return 0			检查成功, 非脏话
			1		脏话符合中文关键字
			2		脏话符合英文关键字
			3		脏话符合中英混合关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/**
  * tDirtyCheckChnEx	GBK字符集中文脏字检查接口
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @param iBegIdx	返回脏字起始下标
  * @param iEndIdx	返回脏话终止下标
  * @return 0			检查成功, 非脏话
			1		脏话符合中文关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/**
  * tDirtyCheckEngEx	GBK字符集英文脏字检查接口
  *
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @param iBegIdx	返回脏字起始下标
  * @param iEndIdx	返回脏话终止下标
  * @return 0 			检查成功, 非脏话
			1 		脏话符合英文关键字
			-1 		未初始化错误
			-2 		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/**
 * tMakeMapCode 	把字符转换为相应的字形映射码和字音映射码
 * @param pszCharacter     源字符串gbk字符集
 * @param iMaxCodeLenth	   输出的映射码字符串的最大限制长度
 * @param pszSpellCode	   转换后的字形映射码字符串
 * @param pszPinyinCode    转换后的字音映射码字符串
 * @return 0	 转换成功
           1	 部分转换成功，输出缓冲区太小
          -1     输入参数错误
 * @note 	所有参数指针不能为NULL，iMaxCodeLenth须为正值
 */

int tMakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth);

/**
  * tDirtyFini tdirtyapi finish接口
  *
  * @param
  * @return
  */

int tDirtyFini();

/**
  * utf8_strlen UTF-8字符真实长度计算
  *
  * @param s UTF-8编码字符串
  * @return > 0 UTF-8编码长度
  */

size_t utf8_strlen(const char *s);

/**
  * utf8_strstr UTF-8字符串查找函数
  *
  * @param s1 UTF-8编码字符串1
  * @param s2 UTF-8编码字符串2
  * @param n s2串长度
  * @return s2串在s1串地址
  */

const char* utf8_strstr(const char *s1, const char *s2, size_t n);

/**
  * utf8SpecialCharFilterInit UTF-8字符集黑名单初始化接口
  *
  * @param piHandle 创建黑名单handle
  * @param pszFileName 白名单文件
  * @return 0 成功-1 失败
  */

int utf8SpecialCharFilterInit(int* piHandle, const char * pszFileName);

/**
  * utf8SpecialCharFilterScan UTF-8字符集黑名单过滤接口
  *
  * @param iHandle 黑名单handle
  * @param pszString 过滤字符
  * @return 0  过滤串不存在黑名单字符1 不存在黑名单字符
  */

int utf8SpecialCharFilterScan(int iHandle, const char *pszString);

/**
  * utf8SpecialCharFilterFini UTF-8字符集黑名单释放接口
  *
  * @param iHandle 黑名单handle
  * @return
  */

void utf8SpecialCharFilterFini(int* piHandle);

/**
  * tdirtyap i初始化接口主要用于win32客户端环境
  *
  * @param pszBuff	脏字表内存映像指针必须\0结尾
  * @param iBuff		脏字表内存映像长度
  * @return 	0		成功
			-1		分配内存失败
  */

int tInitializeDirtyCheckEx(char * pszBuff,  int iBuff);

/**
  * tdirtyapi			线程安全初始化接口
  *
  * @param pszDirtyFile	脏字文件路径, 传入NULL表示从tagent初始化
  * @param uiBusid		分配给各个项目的业务id
  * @return >   0		成功返回句柄
			-1		分配内存失败
			-2		调用tagent  失败
			-3		文件复制失败
  */
int tInitializeDirtyCheck_r	(char * pszFile, unsigned int uiBusid);

/**
  * tDirtyFini_r tdirtyapi  线程安全finish接口
  *
  * @param	iHandle	初始化返回句柄
  * @return
  */

int tDirtyFini_r				(int iHandle);

/**
  * tDirtyCheck		GBK字符集脏字综合检查线程安全接口
  *
  * @param iHandle	初始化线程句柄
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @return 0			检查成功非脏话
			1		脏话符合中文关键字
			2		脏话符合英文关键字
			3		脏话符合中英混合关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheck_r			(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyReplace		GBK字符集脏字综合替换线程安全接口
  *
  * @param iHandle	初始化线程句柄
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel	返回脏话级别
  * @return 0 			过滤成功, 如果存在脏字将pszContent中脏字替换为*
			-1 		未初始化错误
			-2 		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyReplace_r			(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);


/**
  * tDirtyCheckReg_r	GBK字符集正则表达式线程安全检查接口win32无效
  *
  * @param iHandle	初始化线程句柄
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel  返回脏话级别
  * @return 0			检查成功, 非脏话
			1		脏话符合正则关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckReg_r		(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyCheckPiny_r	GBK字符集正则表达式检查接口win32无效
  *
  * @param iHandle	初始化线程句柄
  * @param iDoorLevel	门限脏话级别
  * @param pszContent	输入字符串GBK字符集
  * @param iDirtyLevel  返回脏话级别
  * @return 0			检查成功, 非脏话
			1		脏话符合正则关键字
			-1		未初始化错误
			-2		脏字库无效错误
			-100	初始化无效错误
  */

int tDirtyCheckPiny_r		(int iHandle, int iDoorLevel, char * pszContent, int * iDirtyLevel);

/**
  * tDirtyUpdateCheck_r	手动更新脏字接口, 调用该接口程序检查脏字更新
  *
  * @param iHandle	初始化线程句柄
  * @return
  */

int tDirtyUpdateCheck_r	(int iHandle);

#ifdef __cplusplus
}
#endif

#endif /* __TDIRTYAPI_H__ */

