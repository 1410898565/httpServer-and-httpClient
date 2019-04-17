CROSS_COMPILER=

TARGET=httpClient

CC=$(CROSS_COMPILER)gcc
LD=$(CROSS_COMPILER)g++ 
CPP=$(CROSS_COMPILER)g++

# 配置目录
DIR_PRJ:=$(shell pwd)
DIR_SRC:=$(DIR_PRJ)/src
DIR_INC:=$(DIR_PRJ)/inc

DIR_LIB:=$(DIR_PRJ)/libs
DIR_LIB_CURL:=$(DIR_LIB)/curl/lib

# *.h头文件
INCLUDE+=-I$(DIR_PRJ)/
INCLUDE+=-I$(DIR_INC)/
INCLUDE+=-I$(DIR_LIB)/curl/include/curl

# *.c源码
SRC:=$(wildcard $(DIR_PRJ)/*.c)
SRC+=$(wildcard $(DIR_SRC)/*.c)

# 编译flag
CFLAGS+=-g -I$(INCLUDE) 
CPPFLAGS+=-g -I$(INCLUDE) 

# 链接flag
LDFLAGS+=-O2 -I$(INCLUDE) -lpthread -lm -lrt -lstdc++


# 库
LIBS+= 
STATIC_OLIB+= $(DIR_LIB_CURL)/libcurl.a


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

