###############################################################################
#
# Generic Makefile for C/C++ Program
##################################################################
include ../comm.mk

.PHONY : all clean cleanall

TARGET = libnanopb_s2s.a

CXX = g++
CXXFLAGS = -g -O0 -Wall -Werror -DDEBUG -static

INCS = $(INC_PROTOBUF) $(INC_NANOPB) $(LIB_NANOPB)
SRCS = $(wildcard *.c) 
OBJS = $(patsubst %.c, %.o, $(SRCS))
DEPS = $(patsubst %.c, %.d, $(SRCS))

all : $(TARGET)

%.d : %.c
	$(CXX) -MM $(CXXFLAGS) $(INCS) $< > $@.$$$$; \
	sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.c
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCS) $<

-include $(DEPS)

$(TARGET) : $(OBJS)
	ar rcs $@ $^

clean :
	rm -f $(TARGET) *.o *.d

cleanall : clean
	rm -f *.h
	rm -f *.c
