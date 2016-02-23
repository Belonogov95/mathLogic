#include "main.h"
#include "parser.h"

struct Deduction {
    vector < Line > * lines;
    string term;
    vector < Line > * answer;
    vector < Node * > axiom;
    bool use[3];
    ull c[3];

    Deduction(vector < Line > * lines, string term1);

    string replace(string s, char u, string t);
    void add(string tmp);

    void makeAA();

    Node * myParse(string tmp) ;

    void init() ;

    bool compAx(Node * v, Node * ax, int level);
    
    vector < Line > * go();
};









