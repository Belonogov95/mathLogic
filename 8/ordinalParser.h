#include "main.h"


#ifndef ORDINAL_PARSER
#define ORDINAL_PARSER


struct Node {
    string type;
    long long val;
    shared_ptr < Node > l, r;
    Node(string type, shared_ptr < Node > l, shared_ptr < Node > r);
    Node(string type, long long val); 
};

struct OrdinalParser {
    string data; 
    int cur; 
    OrdinalParser(string data): data(data), cur(0) { }
     
    string nextToken();
    void shiftCur();
    shared_ptr < Node > parseExp();  
    shared_ptr < Node > parseSummand();
    shared_ptr < Node > parseMultiplier();
    shared_ptr < Node > parseTerm();
};

string removeSpace(string s);
vector < string > split(string s, string pattern);

shared_ptr < Node > parseOrdinal(string s);

#endif

