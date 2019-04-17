COMPILE_COMPILER=

TARGET=httpserver

CC=$(CROSS_COMPILER)gcc
LD=$(CROSS_COMPILER)g++ 
CPP=$(CROSS_COMPILER)g++

# 配置目录
DIR_PRJ:=$(shell pwd)
DIR_SRC:=$(DIR_PRJ)/src
DIR_INC:=$(DIR_PRJ)/inc

DIR_LIB:=$(DIR_PRJ)/libs
DIR_LIB_EVENT:=$(DIR_PRJ)/libs/event/lib
# *.h头文件
INCLUDE+=-I$(DIR_PRJ)/
INCLUDE+=-I$(DIR_INC)/

INCLUDE+=-I$(DIR_LIB)/event/include
INCLUDE+=-I$(DIR_LIB)/event/include/event2

# *.c源码
SRC:=$(wildcard $(DIR_PRJ)/*.c)
SRC+=$(wildcard $(DIR_SRC)/*.c)

# 编译flag
CFLAGS+=-g -I$(INCLUDE) 
CPPFLAGS+=-g -I$(INCLUDE) 

# 库
LIBS=-lpthread -lm 
LIBS+=
STATIC_OLIB+= $(DIR_LIB_EVENT)/libevent.a

# 链接flag
LDFLAGS+=-g -I$(INCLUDE) 

OBJS := $(SRC:.c=.o)
DEP := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET):$(SRC) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(STATIC_OLIB) $(LIBS) -o $@

-include $(DEP)
%.o:%.c
	$(CC) $(INCLUDE) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o, %.d, $@) $<
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@
	
%.o:%.cpp
	$(CPP) $(INCLUDE) $(CPPFLAGS) -c $< -o $@
    
clean :
	rm -f $(TARGET) $(OBJS) $(DEP) *.o

