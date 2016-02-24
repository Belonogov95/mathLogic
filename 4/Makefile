
CXXFLAGS= -std=c++11 -O2 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address

all: 4 

4: 4.cpp parser.cpp parser.h main.h
	g++ $(CXXFLAGS) -o bin/4 4.cpp parser.cpp


clean:
	rm bin/4


