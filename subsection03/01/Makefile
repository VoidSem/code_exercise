#
# Makefile
#
#

#define var

CC = gcc
TARGET = ScanDemo
CFLAGS =

#files
objects = $(TARGET).o ScanDir.o
headers = ScanDir.h

#rules
$(TARGET): $(objects)
	$(CC)  $(CFLAGS) $^ -o $@

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
