

CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address
#CXXFLAGS= -std=c++11 

all: 5

parser.o: parser.cpp 
	g++ $(CXXFLAGS) -c $?

5.o: 5.cpp 
	g++ $(CXXFLAGS) -c $?

5: 5.o parser.o parser.h 
	g++ $(CXXFLAGS) -o $@ 5.o parser.o

clean:
	rm -f bin/*
	rm -f *.o 

