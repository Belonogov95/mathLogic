#include "main.h"

#ifndef PARSER_H
#define PARSER_H

const int P = 997;
const int MOD = 1e9 + 7;

string removeSpace(string s);

extern vector < long long > deg;

long long getPow(int pos);

struct Hash {
    long long val;
    int cnt;
    Hash(): val(0), cnt(0) { }
    Hash(string s) {
        val = 0;
        for (int i = 0; i < (int)s.size(); i++) 
            val = (val + s[i] * getPow(i)) % MOD;
        cnt = s.size();
    }
    Hash(long long val, int cnt): val(val), cnt(cnt) { }
    Hash operator + (Hash other) {
        return Hash((val + other.val * getPow(cnt)) % MOD, cnt + other.cnt);    
    }
    Hash addBracket() {
        return Hash("(") + (*this) + Hash(")");
    }
};

struct Node {
    string type;
    Node * l, * r;
    string val;
    vector < Node * > ch;
    Hash hash;
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

