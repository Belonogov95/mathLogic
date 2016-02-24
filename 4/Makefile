
CXXFLAGS= -std=c++11 -O2 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address 

all: 4 

4: 4.cpp parser.cpp main.h parser.h
	g++ $(CXXFLAGS) -o $@ 4.cpp parser.cpp

clean:
	rm solve


