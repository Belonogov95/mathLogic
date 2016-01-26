#include "main.h"

#ifndef PARSER_H
#define PARSER_H


typedef unsigned long long ull;

const int P = 997;

string inBracket(string s);
bool isLetter(string s);

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


    Token nextToken();
    void shiftCur();
    Node * parseImplication();
    Node * parseConjunct();
    Node * parseDisjunct();
    Node * parseAll();
    Node * parse();
    void printTree(Node * v, int tab);

};

struct Line {
    vector < string > assumption;
    Node * head;
    Line (string s);
    Line (Node * head): head(head) { }
    void print(bool flag) {
        if (flag) {
            for (int i = 0; i < (int)assumption.size(); i++) {
                cout << assumption[i];
                if (i + 1 != (int)assumption.size())
                    cout << ",";
            }
            cout << "|-";
        }
        cout << head->s << endl; 
    }
};
#endif 

