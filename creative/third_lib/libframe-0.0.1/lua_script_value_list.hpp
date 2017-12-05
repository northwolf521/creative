#ifndef __LUA_SCRIPT_VALUE_LIST_H__
#define __LUA_SCRIPT_VALUE_LIST_H__

/************************************************************************/
/*                           LUA脚本值列表                              */
/*                                                                      */
/* 脚本值列表中最多允许保存MaxValueCount个值                            */
/*此类适用于在与脚本交互中的函数调用参数                                */
/* 表的传递以及返回值列表的获取操作的场合。                             */
/*                                                                      */
/************************************************************************/

extern "C" {
#include <lua.h>
}

#include "lua_script_value.hpp"

class CLuaScriptValueList {
public:
    static const int MaxValueCount = 16;    //脚本值列表允许存储的值数量上限

public:
    CLuaScriptValueList() {
        m_nCount = 0;
    }
    CLuaScriptValueList(const CLuaScriptValueList& valList) {
        m_nCount = 0;
        operator = (valList);
    }
    /**
     * 获取列表中的值数量
     * @return [description]
     */
    inline int count() {
        return m_nCount;
    }
    /**
     * 将列表转换为ScriptValue数组的运算符重载
     */
    inline operator CLuaScriptValue* () {
        return m_Values;
    }
    /**
     * [getScriptValue description]
     * @param  nIdx [description]
     * @return      [description]
     */
    inline CLuaScriptValue* getScriptValue(int nIdx) {
        return (nIdx >= 0 && nIdx < m_nCount ? &(m_Values[nIdx]) : NULL);
    }
    /**
     * 添加一个值
     */
    template <typename T>
    inline CLuaScriptValueList& operator << (const T val) {
        if (m_nCount < MaxValueCount) {
            m_Values[m_nCount] = val;
            m_nCount++;
        }

        return *this;
    }
    /**
     * 从另一个列表中赋值
     */
    inline CLuaScriptValueList& operator = (const CLuaScriptValueList &valList) {
        clear();

        for (int i=valList.m_nCount-1; i>-1; --i) {
            m_Values[i] = valList.m_Values[i];
        }

        m_nCount = valList.m_nCount;
        return *this;
    }
    /**
     * 累加操作
     */
    inline CLuaScriptValueList& operator += (const CLuaScriptValueList &valList) {
        for (int i = 0; i < valList.m_nCount && m_nCount < MaxValueCount; i++) {
            m_Values[m_nCount++] = valList.m_Values[i];
        }

        return *this;
    }
    /**
     * 清空值列表
     */
    inline void clear() {
        for (int i=m_nCount-1; i>-1; --i) {
            m_Values[i].clear();
        }
        m_nCount = 0;
    }
    /**
     * 将值列表中的值压入lua脚本中。将列表中的值压入脚本中后，列表不会被清空
     */
    void pushArgs(lua_State *pLua, int nCount) {
        for (int i=0; i<nCount; ++i) {
            CLuaScriptValue &value = m_Values[i];
            switch (value.getType()) {
            case CLuaScriptValue::vBool:
                lua_pushboolean(pLua, value.operator bool());
                break;
            case CLuaScriptValue::vLightData: {
                const void* ptr = value;
                if (ptr)
                    lua_pushlightuserdata(pLua, (void*)ptr);
                else
                    lua_pushnil(pLua);
            }
            break;
            case CLuaScriptValue::vNumber:
                lua_pushnumber(pLua, value.operator double());
                break;
            case CLuaScriptValue::vString:
                lua_pushlstring(pLua, value.operator const char*(), value.getSize());
                break;
            default:
                lua_pushnil(pLua);
                break;
            }
        }
    }
    /**
     * 从lua脚本中取出返回值到值列表中，函数返回取出了多少个值。
     * 如果列表中已经存在值，则之前的值将不会清空。
     */
    int getResults(lua_State *pLua, int nCount) {
        int nType, Result = 0;
        while (nCount > 0) {
            if (m_nCount >= MaxValueCount)
                break;

            nType = lua_type(pLua, -nCount);
            switch(nType) {
            case LUA_TBOOLEAN:
                m_Values[m_nCount++] = (lua_toboolean(pLua, -nCount) != 0);
                break;
            case LUA_TLIGHTUSERDATA:
                m_Values[m_nCount++] = lua_touserdata(pLua, -nCount);
                break;
            case LUA_TNUMBER:
                m_Values[m_nCount++] = lua_tonumber(pLua, -nCount);
                break;
            case LUA_TSTRING:
                m_Values[m_nCount++] = lua_tostring(pLua, -nCount);
                break;
            default:
                m_Values[m_nCount++].clear();
                break;
            }

            nCount--;
            Result++;
        }

        lua_pop(pLua, (int)(Result + nCount));
        return Result;
    }

    /**
     * 将lua中传入的参数保存到值列表中。
     * 列表会被清空，从而存放新的值列表
     * 函数返回读取了多少个参数
     */
    int getArguments(lua_State *pLua, int nStackIdx) {
        int nType, Result = 0;
        int nCount = lua_gettop(pLua);

        clear();

        while (nStackIdx <= nCount) {
            if (m_nCount >= MaxValueCount)
                break;

            nType = lua_type(pLua, (int)nStackIdx);
            switch(nType) {
            case LUA_TBOOLEAN:
                m_Values[m_nCount++] = (lua_toboolean(pLua, (int)nStackIdx) != 0);
                break;
            case LUA_TLIGHTUSERDATA:
                m_Values[m_nCount++] = lua_touserdata(pLua, (int)nStackIdx);
                break;
            case LUA_TNUMBER:
                m_Values[m_nCount++] = lua_tonumber(pLua, (int)nStackIdx);
                break;
            case LUA_TSTRING:
                m_Values[m_nCount++] = lua_tostring(pLua, (int)nStackIdx);
                break;
            default:
                m_Values[m_nCount++].clear();
                break;
            }

            nStackIdx++;
            Result++;
        }

        return Result;
    }


private:
    CLuaScriptValue m_Values[MaxValueCount];//值内存块
    int m_nCount;//值数量
};

#endif//LUA_SCRIPT_VALUE_LIST_H_
