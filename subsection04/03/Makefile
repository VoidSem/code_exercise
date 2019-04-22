#
# Makefile
#
#

#define var

CXX = g++
TARGET1 = ServerDemo
TARGET2 = ClientDemo
CXXFLAGS = -std=c++11 -pthread

all:  $(TARGET1) $(TARGET2)
#files
objects = $(TARGET1).o Server.o Client.o
headers = Server.h Common.h Client.h

#rules
$(TARGET1): $(objects)
	$(CXX)  $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CXXFLAGS) $< -o $@
#files
OBJECTS = $(TARGET2).o Client.o
HEADERS = Client.h COmmon.h
$(TARGET2): $(OBJECTS)
	$(CXX)  $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET1) $(TARGET2) $(objects) $(OBJECTS)
