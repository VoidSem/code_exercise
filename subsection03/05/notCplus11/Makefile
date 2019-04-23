#
# Makefile
#
#

CXX=g++
TARGET=PthreadDemo
CXXFLAGS = -pthread

#files
objects = $(TARGET).o MultiPthread.o
headers = MultiPthread.h

#rules
$(TARGET): $(objects)
	$(CXX)  $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(objects) $(TARGET)
