#include "main.h"

typedef unsigned long long ull;

const int P = 997;

struct Node {
    string type;
    //int type; // ! 4 ... & 3 ... | 2 ... -> 1
    Node * l, * r;
    ull hash;
    string s;
    Node(string type, Node * l, Node * r);
};

struct Parser {
    int cur;
    string data;
    Parser(string data);
};

struct Line {
    vector < string > assumption;
    Node * head;
    Line (Node * head): head(head) { }
};

