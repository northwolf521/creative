#ifndef __LUA_SCRIPT_VALUE_H__
#define __LUA_SCRIPT_VALUE_H__

/************************************************************************/
/*                                                                      */
/*                            LUA脚本值对象类                           */
/*                                                                      */
/* LUA中含有nil、bool、number、string、lightuserdata等类型的值          */
/* 该类可用于描述和存储这些值中任意类型的值。这个类通常在与脚本         */
/* 交互的过程中作为脚本函数调用的参数以及存储脚本返回值的时候使用。     */
/*                                                                      */
/************************************************************************/

extern "C" {
#include <lua.h>
}

#include <cstring>
#include <cstdlib>

class CLuaScriptValue {
public:
    /**
    * 定义脚本值的数据类型
    */
    enum ScriptValueType {
        vNull      = LUA_TNIL,           //nil值
        vBool      = LUA_TBOOLEAN,       //bool值
        vLightData = LUA_TLIGHTUSERDATA, //纯指针值
        vNumber    = LUA_TNUMBER,        //number值
        vString    = LUA_TSTRING,        //string值
    };

    /**
    * 定义脚本值的值数据结构
    */
    union ScriptValueData {
        bool   boVal;     //bool值
        void*  pLightPtr; //指针值
        double numVal;    //number值
        struct {
            char*  str;   //字符串指针
            size_t dwLen; //字符串长度
        } strVal;
    };

public:
    CLuaScriptValue() {
        m_Type = vNull;
        memset(&m_Data, 0, sizeof(m_Data));
    }

    ~CLuaScriptValue() {
        clear();
    }

    /**
     * 清除数据
     */
    inline void clear() {
        if (m_Type != vNull) {
            if (m_Type == vString) {
                free(m_Data.strVal.str);
            }

            m_Type = vNull;
            memset(&m_Data, 0, sizeof(m_Data));
        }
    }

    /**
     * 获取数据类型
     * @return [description]
     */
    inline ScriptValueType getType() {
        return m_Type;
    }

    /**
     * 获取数据大小
     * 对于vNull类型的值，返回值为0,
     * 对于vBool类型的值，返回值为sizeof(bool),
     * 对于vLightData类型的值，返回值是sizeof(LPCVOID),
     * 对于vNumber类型的值，返回值是sizeof(double),
     * 对于vString类型的值，返回值是字符串的长度,
    */
    inline size_t getSize() {
        switch(m_Type) {
        case vBool:
            return sizeof(m_Data.boVal);
        case vLightData:
            return sizeof(m_Data.pLightPtr);
        case vNumber:
            return sizeof(m_Data.numVal);
        case vString:
            return m_Data.strVal.dwLen;
        default:
            return 0;
        }
    }

public:
    /**
     * 从int值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (int val) {
        clear();
        m_Type = vNumber;
        m_Data.numVal = (double)val;
        return *this;
    }
    /**
     * 从unsigned int值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (unsigned int val) {
        clear();
        m_Type = vNumber;
        m_Data.numVal = (double)val;
        return *this;
    }
    /**
     * 从int64值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (long long val) {
        clear();
        m_Type = vNumber;
        m_Data.numVal = (double)val;
        return *this;
    }
    /**
     * 从unsigned int64值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (unsigned long long val) {
        clear();
        m_Type = vNumber;
        m_Data.numVal = (double)val;
        return *this;
    }
    /**
     * 从double值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (double val) {
        clear();
        m_Type = vNumber;
        m_Data.numVal = val;
        return *this;
    }
    /**
     * 从float值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (float val) {
        clear();
        m_Type = vNumber;
        m_Data.numVal = val;
        return *this;
    }
    /**
     * 从bool值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (bool val) {
        clear();
        m_Type = vBool;
        m_Data.boVal = val;
        return *this;
    }
    /**
     * 从const char*值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (const char* val) {
        clear();
        m_Type = vString;
        m_Data.strVal.dwLen = strlen(val);
        m_Data.strVal.str = (char*)malloc(m_Data.strVal.dwLen + 1);
        memcpy(m_Data.strVal.str, val, m_Data.strVal.dwLen);
        m_Data.strVal.str[m_Data.strVal.dwLen] = 0;
        return *this;
    }
    /**
     * 从char*值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (char* val) {
        return operator = ((const char*)val);
    }
    /**
     * 从任意类型的指针值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (void* val) {
        clear();
        m_Type = vLightData;
        m_Data.pLightPtr = val;
        return *this;
    }
    /**
     * 从ScriptValueData值的赋值运算函数
     */
    inline CLuaScriptValue& operator = (const CLuaScriptValue& val) {
        switch(val.m_Type) {
        case vBool:
            return operator = (val.m_Data.boVal);
        case vLightData:
            return operator = (val.m_Data.pLightPtr);
        case vNumber:
            return operator = (val.m_Data.numVal);
        case vString:
            return operator = (val.m_Data.strVal.str);
        default:
            clear();
            return *this;
        }
    }

public:
    /**
     * 转换为int的运算符重载
     * @return [description]
     */
    inline operator int () {
        if (m_Type == vNumber)
            return (int)m_Data.numVal;

        if (m_Type == vString && m_Data.strVal.str)
            return atoi(m_Data.strVal.str);

        return 0;
    }
    /**
     * 转换为unsigned int的运算符重载
     * @return [description]
     */
    inline operator unsigned int () {
        if (m_Type == vNumber)
            return (unsigned)m_Data.numVal;

        if (m_Type == vString && m_Data.strVal.str)
            return strtoul(m_Data.strVal.str, NULL, 0);

        return 0;
    }
    /**
     * 转换为int64的运算符重载
     * @return [description]
     */
    inline operator long long () {
        if (m_Type == vNumber)
            return (long long)m_Data.numVal;

        if (m_Type == vString && m_Data.strVal.str)
            return atoll(m_Data.strVal.str);

        return 0;
    }
    /**
     * 转换为uint64的运算符重载
     * @return [description]
     */
    inline operator unsigned long long () {
        if (m_Type == vNumber)
            return (unsigned long long)m_Data.numVal;

        if (m_Type == vString && m_Data.strVal.str)
            return strtoull(m_Data.strVal.str, NULL, 0);

        return 0;
    }
    /**
     * 转换为double的运算符重载
     * @return [description]
     */
    inline operator double () {
        if (m_Type == vNumber)
            return m_Data.numVal;

        if (m_Type == vString && m_Data.strVal.str)
            return strtod(m_Data.strVal.str, NULL);

        return 0;
    }
    /**
     * 转换为float的运算符重载
     * @return [description]
     */
    inline operator float () {
        if (m_Type == vNumber)
            return (float)m_Data.numVal;

        if (m_Type == vString && m_Data.strVal.str)
            return atof(m_Data.strVal.str);

        return 0;
    }
    /**
     * 转换为bool的运算符重载
     * @return [description]
     */
    inline operator bool () {
        switch(m_Type) {
        case vBool:
            return m_Data.boVal;
        case vLightData:
            return m_Data.pLightPtr != NULL;
        case vNumber:
            return m_Data.numVal != 0;
        case vString:
            return m_Data.strVal.str && strcasecmp("true", m_Data.strVal.str) == 0;
        default:
            return false;
        }
    }
    /**
     * 转换为const char*的运算符重载
     */
    inline operator const char* () {
        return operator char* ();
    }
    /**
     * 转换为char*的运算符重载
     */
    inline operator char* () {
        if ( m_Type == vString )
            return m_Data.strVal.str;

        return NULL;
    }
    /**
     * 转换为void*的运算符重载
     */
    inline operator void* () {
        if ( m_Type == vLightData )
            return m_Data.pLightPtr;

        if ( m_Type == vString )
            return m_Data.strVal.str;

        return NULL;
    }

private:
    //类型对应id
    ScriptValueType m_Type;
    //类型对应值
    ScriptValueData m_Data;
};

#endif//LUA_SCRIPT_VALUE_H_
