#include "main.h"
#include "parser.h"


int main() {
    freopen("in", "r", stdin);

    string s;
    getline(cin, s);
    Parser parser(s);
    
    Node * v = parser.parseExpr();

    printTree(v, 0);

}


