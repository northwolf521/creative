#!/bin/bash
cd `dirname $0`
source ./comm.sh

ulimit -c unlimited

PROC_START_FLAG=0
#############################
if [ "start" == "$1" ]; then
    PROC_START_FLAG=1
fi

#############################
function check_and_setup() 
{
    PROC_NAME=$1
    PROC_PATH=$2
    PROC_GUID=$3

    PROC_STATE=0
    if [ -f "${PROC_PATH}/pid.txt" ] && [ -d "/proc/`cat ${PROC_PATH}/pid.txt`" ]; then
        echo "${PROC_PATH}/${PROC_NAME} is alive."
        PROC_STATE=1
    else
        echo "${PROC_PATH}/${PROC_NAME} is deaded."
        PROC_STATE=0
    fi

    PROC_SETUP_ARGS=1
    if [ $PROC_START_FLAG == 1 ] && [ $PROC_STATE == 0 ]; then
        cd ${PROC_PATH}
        ./$PROC_NAME $PROC_CONFIG_FILE $PROC_LOG_FILE $PROC_SETUP_ARGS $PWD $PROC_RELOAD_FILE $PROC_GUID
        cd ..

        FLAGS=0
        
        declare -i i=0
        while ((i<10)); do
            if [ -f "${PROC_PATH}/pid.txt" ] && [ -d "/proc/`cat ${PROC_PATH}/pid.txt`" ]; then
                FLAGS=1
                break
            fi
            sleep 1
            let ++i
        done

        if [ $FLAGS -eq 0 ]; then
            echo "${PROC_PATH}/${PROC_NAME} has timeout"
        else
            echo "${PROC_PATH}/${PROC_NAME} has started"
        fi
    fi
    
    return 1
}

#############################
CONN_NAME=conn_srv
CONN_PATH=conn_srv
check_and_setup $CONN_NAME $CONN_PATH
if [ $? -eq 0 ]; then
    exit 0
fi

#############################
for HOST in $DBPROC_HOST_LIST; do
    DBPROC_NAME=dbproc_srv
    DBPROC_PATH=dbproc_s${HOST}
    check_and_setup $DBPROC_NAME $DBPROC_PATH $HOST
    if [ $? -eq 0 ]; then
        exit 0
    fi
done

#############################
for HOST in $BATTLE_HOST_LIST; do
    BATTLE_NAME=battle_srv
    BATTLE_PATH=battle_s${HOST}
    check_and_setup $BATTLE_NAME $BATTLE_PATH $HOST
    if [ $? -eq 0 ]; then
        exit 0
    fi
done

#############################
for HOST in $CACHE_HOST_LIST; do
    CACHE_NAME=cache_srv
    CACHE_PATH=cache_s${HOST}
    check_and_setup $CACHE_NAME $CACHE_PATH $HOST
    if [ $? -eq 0 ]; then
        exit 0
    fi
done

#############################
AGENT_NAME=agent_srv
AGENT_PATH=agent_srv
check_and_setup $AGENT_NAME $AGENT_PATH
if [ $? -eq 0 ]; then
    exit 0
fi

#############################
CROSS_NAME=cross_srv
CROSS_PATH=cross_srv
check_and_setup $CROSS_NAME $CROSS_PATH
if [ $? -eq 0 ]; then
    exit 0
fi

#############################
for HOST in $GAME_HOST_LIST; do
    GAME_NAME=game_srv
    GAME_PATH=game_s${HOST}
    check_and_setup $GAME_NAME $GAME_PATH $HOST
    if [ $? -eq 0 ]; then
        exit 0
    fi
done

#############################
for HOST in $LOGIN_HOST_LIST; do
    LOGIN_NAME=login_srv
    LOGIN_PATH=login_s${HOST}
    check_and_setup $LOGIN_NAME $LOGIN_PATH $HOST
    if [ $? -eq 0 ]; then
        exit 0
    fi
done

#############################
RANK_NAME=rank_srv
RANK_PATH=rank_srv
check_and_setup $RANK_NAME $RANK_PATH
if [ $? -eq 0 ]; then
    exit 0
fi

#############################
MAIL_NAME=mail_srv
MAIL_PATH=mail_srv
check_and_setup $MAIL_NAME $MAIL_PATH
if [ $? -eq 0 ]; then
    exit 0
fi

#############################
SOCIAL_NAME=social_srv
SOCIAL_PATH=social_srv
check_and_setup $SOCIAL_NAME $SOCIAL_PATH
if [ $? -eq 0 ]; then
    exit 0
fi

#############################
REPORT_NAME=report_srv
REPORT_PATH=report_srv
check_and_setup $REPORT_NAME $REPORT_PATH
if [ $? -eq 0 ]; then
    exit 0
fi


