#include "main.h"

#ifndef PARSER_H
#define PARSER_H

const int P = 997;

string removeSpace(string s);

vector < string > split(string s, string pattern);

string addBracket(string s);

string replace(string s, char u, string t);


struct Node {
    string type;
    Node * l, * r;
    vector < Node * > ch;
    string s;
    ull hash;
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

#endif

