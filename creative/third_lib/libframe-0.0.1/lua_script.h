#ifndef __LUA_SCRIPT_H__
#define __LUA_SCRIPT_H__

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <iostream>
#include <string>
#include "lua_script_value_list.hpp"

class CLuaScript {
public:
    CLuaScript();
    ~CLuaScript();

    /**
     * 执行脚本脚本
     * @param  sText [代码内容]
     * @param  reg   [函数接口列表]
     * @return       [description]
     */
    bool setScript(const char* sText, const luaL_reg* reg = NULL);

    /**
     * 指定脚本路径
     * @param  path [脚本路径]
     * @param  reg  [函数库列表]
     * @return      [description]
     */
    bool setScriptFile(const char* path, const luaL_reg* reg = NULL);

    /**
     * 判断脚本中是否存在名为sFnName的函数
     * @param  sFnName [description]
     * @return         [description]
     */
    bool functionExists(const char* sFnName);

    /**
     * 调用脚本函数，如果脚本调用成功，则函数返回true，否则返回false。
     * 当调用脚本失败时，函数返回false并在返回值列表中存储1个错误描述内容的字符串值
     * @param  sFnName      [lua函数名称]
     * @param  args         [脚本参数列表]
     * @param  results      [脚本返回值列表]
     * @param  nResultCount [期望的返回值数量，LUA_MULTRET表示不限制]
     * @return              [description]
     */
    bool call(const char* sFnName, CLuaScriptValueList* args, CLuaScriptValueList* results, int nResultCount = LUA_MULTRET);

    /**
     * 错误描述信息
     * [getLastErrorDesc description]
     * @return [description]
     */
    const char* getLastErrorDesc() {
        return m_sLastErrDesc.c_str();
    }

protected:
    /**
     * 调用脚本函数错误的处理函数
     * @param sFnName [description]
     */
    void scriptCallError(const char* sFnName);
    /**
     * 创建脚本虚拟机
     * @return [description]
     */
    lua_State* createLuaVM();
    /**
     * 打开基础库。默认会打开base、string、math以及table库。返回true表示成功。
     * @return [description]
     */
    bool openBaseLibs();
    /**
     * 注册本地函数库。返回true表示成功。
     * @param  reg [description]
     * @return     [description]
     */
    bool registLocalLibs(const luaL_reg* reg);

protected:
    //LUA虚拟机对象
    lua_State* m_pLua;
    //上一个错误的描述
    std::string m_sLastErrDesc;
};

#endif //LUA_SCRIPT_H_
