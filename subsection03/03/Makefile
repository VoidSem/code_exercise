#
# Makefile
#
#

#define var

CC = gcc
TARGET = TestSysSignal
CFLAGS =

#files
objects = $(TARGET).o
headers =

#rules
$(TARGET): $(objects)
	$(CC)  $(CFLAGS) $^ -o $@

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
