#
# Makefile
#
#

#define var

CC = gcc
TARGET = TestGetTime
CFLAGS =

#files
objects = TestGetTime.o GetTime.o
headers = GetTime.h

#rules
$(TARGET): $(objects)
	$(CC)  $(CFLAGS) $^ -o $@

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
