#
# Makefile
#
#

#define var

CC = gcc
TARGET1 = sockServer
TARGET2 = sockClient
CFLAGS =

all:  $(TARGET1) $(TARGET2)
#files
objects = $(TARGET1).o
headers = sockCommon.h

#rules
$(TARGET1): $(objects)
	$(CC)  $(CFLAGS) $^ -o $@

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@
#files
OBJECTS = $(TARGET2).o
$(TARGET2): $(OBJECTS)
	$(CC)  $(CFLAGS) $^ -o $@

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET1) $(TARGET2) $(objects) $(OBJECTS)
