#include "main.h"


string removeSpace(string s);


struct Node {
    string type;
    Node * l, * r;
    string val;
    vector < Node * > ch;
    Node(string type, Node * l, Node * r);
    Node(string type, string val);
    Node(string type, string val, vector < Node * > ch);
};

void printTree(Node * v, int tab);


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
    Node * parseTerm();
    Node * parseSummand();
    Node * parseMultiply();


};


