#
# Makefile
#
#

#define var

CXX = g++
TARGET = TestDemo
CXXFLAGS = -pthread -std=c++11

#files
objects = $(TARGET).o Server.o
headers = Server.h Common.h

#rules
$(TARGET): $(objects)
	$(CXX)  $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
