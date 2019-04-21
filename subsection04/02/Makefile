#
# Makefile
#
#

#define var

CXX = g++
TARGET1 = ServerDemo
TARGET2 = ClientDemo
CXXFLAGS = -pthread

all:  $(TARGET1) $(TARGET2)
#files
objects = $(TARGET1).o Server.o
headers = Server.h Common.h

#rules
$(TARGET1): $(objects)
	$(CXX)  $(CFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CFLAGS) $< -o $@
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
