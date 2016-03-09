

#CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address
CXXFLAGS= -std=c++11 

all: 8

ordinalParser.o: ordinalParser.cpp 
	g++ $(CXXFLAGS) -c $?

ordinalCalc.o: ordinalCalc.cpp 
	g++ $(CXXFLAGS) -c $?

8.o: 8.cpp 
	g++ $(CXXFLAGS) -c $?

8: 8.o ordinalParser.o ordinalCalc.o 
	g++ $(CXXFLAGS) -o $@ 8.o ordinalParser.o ordinalCalc.o

clean:
	rm -f bin/*
	rm -f *.o 

