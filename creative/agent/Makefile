##################################################################
include ../comm.mk

.PHONY : all clean

TARGET = agentd

CXX = g++
CXXFLAGS = -g -O0 -Wall -Werror -std=c++11 -Wno-literal-suffix -fpermissive -DDEBUG -D__SERVER__ -static
LDFLAGS =

INCS = $(INC_FRAME) $(INC_LIBEVENT) $(INC_TSF4G) $(INC_NANOPB_S2S) $(INC_NANOPB) $(INC_REDIS)
LIBS = $(LIB_FRAME) $(LIB_LIBEVENT) $(LIB_TSF4G) $(LIB_NANOPB_S2S) $(LIB_NANOPB) $(LIB_REDIS)
SRCS = $(wildcard *.cpp) 
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
DEPS = $(patsubst %.cpp, %.d, $(SRCS))

all : $(TARGET)

%.d : %.cpp
	$(CXX) -MM $(CXXFLAGS) $(INCS) $< > $@.$$$$; \
	sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCS) $<

-include $(DEPS)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

clean :
	rm -f $(TARGET) *.o *.d
