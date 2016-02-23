#include "main.h"


string removeSpace(string s);

struct Node {
    string type;
    Node * l, * r;
    string val;
    Node(string type, Node * l, Node * r);
    Node(string type, string val);
};



struct Parser {
    int cur;
    string data;
    Parser(string data);

    Token nextToken();
    void shiftCur();
    Node * parseConjunct();
    Node * parseDisjunct();
    Node * parseExpr();
    Node * parseUnar();
    Node * parsePredicate();
    Node * parseVar();

    void printTree(Node * v, int tab);

};


