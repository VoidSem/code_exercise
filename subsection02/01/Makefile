#
# Makefile
#
#

CXX=g++
TARGET=SortString
CXXFLAGS = -std=c++11

#files
objects = $(TARGET).o
headers =

#rules
$(TARGET): $(objects)
	$(CXX)  $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(headers)
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
