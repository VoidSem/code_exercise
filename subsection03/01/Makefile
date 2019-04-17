#
# Makefile
#
#

#define var

CC = gcc
TARGET = TestDemo
CFLAGS =

#files
objects = TestDemo.o ScanDir.o
headers = ScanDir.h

#rules
$(TARGET): $(objects)
	$(CC)  $(CFLAGS) $^ -o $@

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
