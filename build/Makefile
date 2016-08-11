SRC = 
TARGET := test

CC := gcc
RM = rm -rf
LOCAL_CFLAGS = -o 
LOCAL_LDFLAGS =  

#############################

.PHONY:all
all: 
	$(CC) $(LOCAL_CFLAGS) $(TARGET1) $(OBJS)

.PHONY: clean install

install:
	mv $(TARGET) $(TARGET1) bin/

clean:
	$(RM) $(TARGET) $(OBJS) $(TARGET1) bin/
