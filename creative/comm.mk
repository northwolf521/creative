INC_FRAME = -I../third_lib/libframe-0.0.1
LIB_FRAME = -L../third_lib/libframe-0.0.1/lib -lframe

INC_LIBEVENT = -I../third_lib/libevent-2.1.8 -I../third_lib/libevent-2.1.8/include 
LIB_LIBEVENT = -L../third_lib/libevent-2.1.8/lib -levent -levent_core -levent_extra -levent_openssl -levent_pthreads

INC_MYSQL = -I/usr/include/mysql
LIB_MYSQL = `mysql_config --libs`

INC_REDIS = -I../third_lib/hiredis-0.11.0/
LIB_REDIS = -L../third_lib/hiredis-0.11.0/lib -lhiredis

INC_NANOPB = -I../third_lib/nanopb-0.3.8/
LIB_NANOPB = -L../third_lib/nanopb-0.3.8/lib -lprotobuf-nanopb

INC_NANOPB_S2S = -I../nanopb/
LIB_NANOPB_S2S = -L../nanopb/ -lnanopb_s2s

INC_LUA = -I../third_lib/lua-5.1/src
LIB_LUA = -L../third_lib/lua-5.1/lib -llua

INC_TSF4G = -I../third_lib/tsf4g/include
LIB_TSF4G = -L../third_lib/tsf4g/lib -ltsf4g -lscew -lexpat -lpthread