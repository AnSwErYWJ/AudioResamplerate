SRC := ./source/resamplerate.c
INI_SRC := ./source/iniparser/iniparser.c ./source/iniparser/dictionary.c
LOG_SRC := ./source/log/log.c
RESAM_SRC := ./source/resamplerate/samplerate.c ./source/resamplerate/src_sinc.c ./source/resamplerate/src_zoh.c ./source/resamplerate/src_linear.c
All_SRC = $(SRC) $(INI_SRC) $(LOG_SRC) $(RESAM_SRC)

OBJS := $(SRC:.c=.o)
INI_OBJS := $(INI_SRC:.c=.o)
LOG_OBJS := $(LOG_SRC:.c=.o)
RESAM_OBJS := $(RESAM_SRC:.c=.o)
All_OBJS := $(SRC:.c=.o) $(INI_SRC:.c=.o) $(LOG_SRC:.c=.o) $(RESAM_SRC:.c=.o)

TARGET := ./bin/resamplerate
INI_TARGET := ./lib/libiniparser.so
LOG_TARGET := ./lib/liblog.so
RESAM_TARGET := ./lib/libresamplerate.so

CC := gcc
PLUS := g++
RM := -rm -rf
CFLAGS := -Wall -O2 -m64 -D_GUN_SOURCE
LDFLAGS = -Llib -liniparser -llog -lresamplerate
LDSHFLAGS = -fPIC -shared
CPPFLAGS = -I./include/log -I ./include/iniparser/ -I ./include/resamplerate/ -I ./include/

rely := $(All_SRC:.c=.d) # 生成.d文件
tmp := ./source/*.d.*

#############################

.PHONY: iniparser log resamplerate clean cleanall cleanso

$(TARGET): $(OBJS)
	$(PLUS) $(CFLAGS) $(LDFLAGS) $^ -o $@

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
$(rely): $(All_SRC)
	@set -e; rm -f $@ &> /dev/null; # 设置错误检查,若下面有语句执行错误则直接退出, 删除旧的.d文件\
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; # 生成依赖文件 \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; # 替换临时文件 \
	rm -f $@.$$$$ &> /dev/null # 删除临时文件

# 替换
-include $(rely)

cleanall : clean cleanso

clean:
	$(RM) $(All_OBJS) $(rely) $(tmp) $(TARGET)

cleanso:
	$(RM) $(INI_TARGET) $(LOG_TARGET) $(RESAM_TARGET)

