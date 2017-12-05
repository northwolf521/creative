#!/bin/bash
source ./comm.sh

SHM_KEY_LIST=`ipcs -m|grep '0x'|awk '{print $1}'`
SHM_KEY1=`grep shm_player_key ${PROC_INI_FILE}|awk -F'=' '{print $2}'|awk '{print $1}'`
SHM_KEY2=`grep shm_legion_key ${PROC_INI_FILE}|awk -F'=' '{print $2}'|awk '{print $1}'`
for key in ${SHM_KEY_LIST[@]}
do
    if [ "${key}"x = "${SHM_KEY1}"x ]; then
        ipcrm -M ${key}
        echo "rm success, key1[${key}]"
        continue
    fi

    if [ "${key}"x = "${SHM_KEY2}"x ]; then
        echo "rm success, key2[${key}]"
        continue
    fi
done
