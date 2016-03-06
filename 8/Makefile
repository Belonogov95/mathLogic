
#CXXFLAGS= -std=c++11 -Wno-unused-result -O3
CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address

all: 8 

8: 8.cpp ordinalParser.cpp ordinalParser.h main.h ordinalCalc.cpp ordinalCalc.h
	g++ $(CXXFLAGS) -o bin/$@ $?

clean:
	rm -f bin/8 





