#
# Makefile
#
#

#define var
CC=gcc
CXX=g++
TARGET=TestDemo
CXXFLAGS = -std=c++11

#files
objects = TestDemo.o StringSort.o
headers = StringSort.h

#rules
$(TARGET): $(objects)
	$(CXX)  $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
