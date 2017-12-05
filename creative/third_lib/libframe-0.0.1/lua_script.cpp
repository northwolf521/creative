#include "log.h"
#include "lua_script.h"

CLuaScript::CLuaScript() {
    m_pLua = NULL;
}

CLuaScript::~CLuaScript() {
    if (m_pLua) {
        lua_close(m_pLua);
        m_pLua = NULL;
    }
}

bool CLuaScript::setScript(const char* sText, const luaL_reg* reg/* = NULL*/) {
    if (m_pLua) {
        lua_close(m_pLua);
        m_pLua = NULL;
    }

    if (sText && sText[0]) {
        m_pLua = createLuaVM();

        openBaseLibs();
        if (reg) {
            registLocalLibs(reg);
        }

        if (luaL_dostring(m_pLua, sText) != 0) {
            return false;
        }
    }

    return true;
}

bool CLuaScript::setScriptFile(const char* path, const luaL_reg* reg/* = NULL*/) {
    if (m_pLua) {
        lua_close(m_pLua);
        m_pLua = NULL;
    }

    if (path && path[0]) {
        m_pLua = createLuaVM();

        openBaseLibs();

        if (reg) {
            registLocalLibs(reg);
        }

        if (luaL_dofile(m_pLua, path) != 0) {
            return false;
        }
    }

    return true;
}

bool CLuaScript::functionExists(const char* sFnName) {
    if (!m_pLua || !sFnName || !sFnName[0])
        return false;

    lua_getglobal(m_pLua, sFnName);
    bool result = lua_isfunction(m_pLua, -1) != 0;
    lua_pop(m_pLua, 1);
    return result;
}

bool CLuaScript::call(const char* sFnName, CLuaScriptValueList* args, CLuaScriptValueList* results, int nResultCount) {
    if (!m_pLua) {
        return false;
    }

    bool bResult = true;
    int nArgCount = args ? args->count() : 0;
    int nTop = lua_gettop(m_pLua);//获取当前的栈顶

    lua_getglobal(m_pLua, sFnName);
    if (nArgCount > 0) {
        args->pushArgs(m_pLua, nArgCount);
    }

    int nErr = lua_pcall(m_pLua, nArgCount, nResultCount, 0);
    if (nErr) {
        scriptCallError(sFnName);
        bResult = false;
    }
    else {
        if (results) {
            results->clear();

            //如果期望的返回值是不限个，则计算本次函数调用的返回值数量
            if (nResultCount == LUA_MULTRET)
                nResultCount = lua_gettop(m_pLua) - nTop;

            if (nResultCount > 0 )
                results->getResults(m_pLua, nResultCount);
        }
    }

    //恢复堆栈
    lua_settop(m_pLua, nTop);
    return bResult;
}

void CLuaScript::scriptCallError(const char* sFnName) {
    m_sLastErrDesc = "func:";
    if (sFnName) {
        m_sLastErrDesc += sFnName;
    }
    else {
        m_sLastErrDesc += "null";
    }

    m_sLastErrDesc += ",err:";

    const char* sErrDesc = NULL;

    //从lua中获取错误描述
    int nTop = lua_gettop(m_pLua);
    if (nTop > 0)
        sErrDesc = lua_tostring(m_pLua, -1);

    if (sErrDesc)
        m_sLastErrDesc += sErrDesc;
    else
        m_sLastErrDesc += "Undefined Error";

    //从lua栈中弹出错误描述
    if (nTop > 0)
        lua_pop(m_pLua, 1);
}

lua_State* CLuaScript::createLuaVM() {
    return luaL_newstate();
}

bool CLuaScript::openBaseLibs() {
    if (m_pLua) {
        luaL_openlibs(m_pLua);
        return true;
    }

    return false;
}

bool CLuaScript::registLocalLibs(const luaL_reg* reg) {
    luaL_register(m_pLua, "LuaHelp", reg);
    return true;
}
