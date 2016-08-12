SRC = ./source/resamplerate.c ./source/samplerate.c ./source/src_sinc.c ./source/src_zoh.c ./source/src_linear.c 
TARGET := ./resamplerate

CC := gcc
RM = rm -rf
LOCAL_CFLAGS = -w -O2 -I ./include/ -o
LOCAL_LDFLAGS =  

#############################

.PHONY:all
all: 
	$(CC) $(LOCAL_CFLAGS) $(TARGET) $(SRC)

.PHONY: clean

clean:
	$(RM) $(TARGET)
