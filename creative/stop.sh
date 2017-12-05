#!/bin/bash
cd `dirname $0`
source ./comm.sh

#############################
function send_proc_signal()
{
    PROC_PATH=$1
    PROC_SIGNAL=$2
    if [ -f "${PROC_PATH}/pid.txt" ] && [ -d "/proc/` cat ${PROC_PATH}/pid.txt`" ]; then
        kill ${PROC_SIGNAL} `cat ${PROC_PATH}/pid.txt`
        echo "send signal($PROC_SIGNAL) to ${PROC_PATH}/${PROC_NAME} succ."
        return 0
    fi

    echo "send signal($PROC_SIGNAL) to ${PROC_PATH}/${PROC_NAME} fail."
    return 1
}

function check_proc_running()
{
    PROC_PATH=$1
    if [ -f "${PROC_PATH}/pid.txt" ] && [ -d "/proc/`cat ${PROC_PATH}/pid.txt`" ]; then
        return 1
    else
        return 0
    fi
}

#############################
SIGNAL_NO=-2

#############################
CONN_PATH=conn_srv
send_proc_signal $CONN_PATH $SIGNAL_NO
if [ $? -eq 1 ]; then
    exit 0
fi

echo "Waiting server stop..."

#############################
declare -i i=0
while true; do
    sleep 1s
    
    check_proc_running $CONN_PATH
    if [ $? -eq 1 ]; then
        echo "Waiting for closing ${i}s ..."
    else
        break
    fi

    let ++i
    if [ $i -ge 300 ]; then
        echo "End of the timeout(${i}s) ..."
        break
    fi
done


#############################
for HOST in $BATTLE_HOST_LIST; do
    BATTLE_PATH=battle_s${HOST}
    send_proc_signal $BATTLE_PATH $SIGNAL_NO
done

#############################
AGENT_PATH=agent_srv
send_proc_signal $AGENT_PATH $SIGNAL_NO

#############################
CROSS_PATH=cross_srv
send_proc_signal $CROSS_PATH $SIGNAL_NO

#############################
for HOST in $DBPROC_HOST_LIST; do
    DBPROC_PATH=dbproc_s${HOST}
    send_proc_signal $DBPROC_PATH $SIGNAL_NO
done

#############################
for HOST in $CACHE_HOST_LIST; do
    CACHE_PATH=cache_s${HOST}
    send_proc_signal $CACHE_PATH $SIGNAL_NO
done

#############################
REPORT_PATH=report_srv
send_proc_signal $REPORT_PATH $SIGNAL_NO

#############################
echo "all srv stoped"
