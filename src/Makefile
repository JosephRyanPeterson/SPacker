# Macros
CXX      = g++
DEBUG    = -g
OBJS     = main.o packer.o region.o sphere.o
CXXFLAGS = -Wall -std=c++11 -c $(DEBUG)
LDFLAGS  = -Wall $(DEBUG)

default: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o spacker

clean:
	\rm *.o spacker
    
