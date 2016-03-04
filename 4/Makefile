
CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address

all: solve 

solve: 4.cpp parser.cpp parser.h
	g++ $(CXXFLAGS) -o bin/4 4.cpp parser.cpp


clean:
	rm solve 





