VPATH ?= ./source/:./include:.

SRC := ./source/resamplerate.c
INI_SRC := ./source/iniparser.c ./source/dictionary.c
LOG_SRC := ./source/log.c
RESAM_SRC := ./source/samplerate.c ./source/src_sinc.c ./source/src_zoh.c ./source/src_linear.c

OBJS := $(SRC:.c=.o)
INI_OBJS := $(INI_SRC:.c=.o)
LOG_OBJS := $(LOG_SRC:.c=.o)
RESAM_OBJS := $(RESAM_SRC:.c=.o)

TARGET := ./bin/resamplerate
INI_TARGET := ./lib/libiniparser.so
LOG_TARGET := ./lib/liblog.so
RESAM_TARGET := ./lib/libresamplerate.so

CC := gcc
RM := -rm -rf
CFLAGS := -Wall -O2
LDFLAGS = -Llib -liniparser -llog -lresamplerate
LDSHFLAGS = -fPIC -shared
CPPFLAGS = -I./include/

rely_src := $(SRC) $(INI_SRC) $(LOG_SRC) $(RESAM_SRC)
rely := $(rely_src:.c=.d) # 生成.d文件

#############################

.PHONY: iniparser log resamplerate clean cleanall cleanso

all : $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

iniparser : $(INI_TARGET)
$(INI_TARGET): $(INI_SRC)
	$(CC) $(LDSHFLAGS) $(CFLAGS) $(CPPFLAGS) $^ -o $@

log : $(LOG_TARGET)
$(LOG_TARGET): $(LOG_SRC)
	$(CC) $(LDSHFLAGS) $(CFLAGS) $(CPPFLAGS) $^ -o $@

resamplerate : $(RESAM_TARGET)
$(RESAM_TARGET): $(RESAM_SRC)
	$(CC) $(LDSHFLAGS) $(CFLAGS) $(CPPFLAGS) $^ -o $@

# 自动生成依赖
$(rely): $(rely_src)
	@set -e; rm -f $@ &> /dev/null; # 设置错误检查,若下面有语句执行错误则直接退出, 删除旧的.d文件\
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; # 生成依赖文件 \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; # 替换临时文件 \
	rm -f $@.$$$$ &> /dev/null # 删除临时文件

# 替换
-include $(rely)

cleanall : clean cleanso

clean:
	$(RM) $(OBJS) $(INI_OBJS) $(LOG_OBJS) $(RESAM_OBJS) $(rely) ./source/*.d.*

cleanso:
	$(RM) $(INI_TARGET) $(LOG_TARGET) $(RESAM_TARGET)





