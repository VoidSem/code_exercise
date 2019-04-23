#
# Makefile
#
#

CXX=g++
TARGET=ThreadDemo
CXXFLAGS = -std=c++11 -pthread

#files
objects = $(TARGET).o MultiThread.o
headers = MultiThread.h

#rules
$(TARGET): $(objects)
	$(CXX)  $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(objects) $(TARGET)
