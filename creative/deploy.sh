#!/bin/sh
cd `dirname $0`
source ./comm.sh

# DIRNAME=$(date +%Y%m%d%H%M%S)
PACKPATH=../deploy
LOGCONFIG=tlogconf.xml
mkdir -p $PACKPATH
# cp -f check.sh $PACKPATH/
# cp -f start.sh $PACKPATH/
# cp -f stop.sh  $PACKPATH/
# cp -f kill.sh  $PACKPATH/
# cp -f comm.sh  $PACKPATH/
# cp -f ipcrm.sh $PACKPATH/

# mkdir -p $PACKPATH/tool/config/
# cp -f ../tool/xls_convert/config/*.xml $PACKPATH/tool/config/

# mkdir -p $PACKPATH/tool/lua/
# cp -rf script/* $PACKPATH/tool/lua/

#conn
mkdir -p $PACKPATH/conn_srv
cp -f conn_srv/conn_srv $PACKPATH/conn_srv/
cp -f misc/release/$LOGCONFIG $PACKPATH/conn_srv/

#game
for i in $GAME_HOST_LIST; do
    mkdir -p $PACKPATH/game_s$i
    cp -f gamed/gamed $PACKPATH/game_s$i/
    cp -f misc/release/$LOGCONFIG $PACKPATH/game_s$i/
done

#agent
mkdir -p $PACKPATH/agentd
cp -f agentd/agentd $PACKPATH/agentd/
cp -f misc/release/$LOGCONFIG $PACKPATH/agentd/

#report
mkdir -p $PACKPATH/report_srv
cp -f report_srv/report_srv $PACKPATH/report_srv/
cp -f misc/release/$LOGCONFIG $PACKPATH/report_srv/

