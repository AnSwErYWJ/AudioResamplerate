DEFAULT_SRC = ./source/resamplerate.c
INI_SRC = ./source/iniparser.c ./source/dictionary.c
LOG_SRC = ./source/log.c
RESAM_SRC = ./source/samplerate.c ./source/src_sinc.c ./source/src_zoh.c ./source/src_linear.c
ALL_SRC = ./source/resamplerate.c ./source/samplerate.c ./source/src_sinc.c ./source/src_zoh.c ./source/src_linear.c ./source/log.c ./source/iniparser.c ./source/dictionary.c

BIN = ./bin/resamplerate
INI_TARGET = ./lib/libiniparser.so
LOG_TARGET = ./lib/liblog.so
RESAM_TARGET = ./lib/libresamplerate.so
CLEAN_TARGET =

CC := gcc
RM = rm -rf
CFLAGS = -w -O2 -I ./include/ -o
LDFLAGS = -Llib -liniparser -llog -lresamplerate
LDSHFLAGS = -fPIC -shared

#############################

.PHONY: default iniparser log resamplerate all clean

default:
	$(CC) $(LDFLAGS) $(CFLAGS) $(BIN) $(DEFAULT_SRC)

resamplerate:
	$(CC) $(LDSHFLAGS) $(CFLAGS) $(RESAM_TARGET) $(RESAM_SRC)

iniparser:
	$(CC) $(LDSHFLAGS) $(CFLAGS) $(INI_TARGET) $(INI_SRC)

log:
	$(CC) $(LDSHFLAGS) $(CFLAGS) $(LOG_TARGET) $(LOG_SRC)

all:
	$(CC) $(CFLAGS) $(BIN) $(ALL_SRC)

clean:
	$(RM) $(CLEAN_TARGET)


