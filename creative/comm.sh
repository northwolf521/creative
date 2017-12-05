#!/bin/bash

#服务配置
PROC_CONFIG_FILE=../server_info.ini
#资源配置
PROC_RELOAD_FILE=../reload_info.ini
#日志配置
PROC_LOG_FILE=./tlogconf.xml
#INI名称
PROC_INI_FILE=${PROC_CONFIG_FILE##*/}

#数据服
DBPROC_HOST_LIST="1"
#战斗服
BATTLE_HOST_LIST="1"
#缓存服
CACHE_HOST_LIST="1"
#游戏服
GAME_HOST_LIST="1"
#登录服
LOGIN_HOST_LIST="1"