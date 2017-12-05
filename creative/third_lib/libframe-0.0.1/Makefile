##################################################################
INC_RES  = -I../../tool/xls_convert/h

INC_PROTOBUF = -I../protobuf-c-0.15/src
LIB_PROTOBUF = -L../protobuf-c-0.15/lib -lprotobuf-c

INC_LIBEVENT = -I../libevent-2.1.8 -I../libevent-2.1.8/include 
LIB_LIBEVENT = -L../libevent-2.1.8/lib -levent -levent_core -levent_extra -levent_openssl -levent_pthreads

INC_MYSQL = -I/usr/include/mysql
LIB_MYSQL = `mysql_config --libs`

INC_REDIS = -I../hiredis-0.11.0/
LIB_REDIS = -L../hiredis-0.11.09/lib -lhiredis

INC_NANOPB = -I../nanopb-0.3.8/
LIB_NANOPB = -L../nanopb-0.3.8/lib -lprotobuf-nanopb

INC_LUA = -I../lua-5.1/src
LIB_LUA = -L../lua-5.1/lib -llua

INC_TSF4G = -I../tsf4g/include
LIB_TSF4G = -L../tsf4g/lib -ltsf4g -lscew -lexpat -lpthread

.PHONY : all clean

TARGET = libframe.a

CXX = g++
CXXFLAGS = -g -O0 -Wall -Werror -std=c++11 -Wno-literal-suffix -DDEBUG -DDEBUG_MEM_POOL -static

INCS = -I. $(INC_RES) $(INC_LIBEVENT) $(INC_LUA) $(INC_TSF4G) $(INC_NANOPB) $(INC_REDIS)
LIBS = -I. $(LIB_RES) $(LIB_LIBEVENT) $(LIB_LUA) $(LIB_TSF4G) $(LIB_NANOPB) $(LIB_REDIS)
SRCS = $(wildcard *.cpp) 
OBJS = $(patsubst %.cpp, %.o,$(SRCS))
DEPS = $(patsubst %.cpp, %.d,$(SRCS))

all : $(TARGET)

%.d : %.cpp
	$(CXX) -MM $(CXXFLAGS) $(INCS) $(LIBS) $< > $@.$$$$; \
	sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCS) $<

-include $(DEPS)

$(TARGET) : $(OBJS)
	ar rcs $@ $^

clean :
	rm -f $(TARGET) *.o *.d
